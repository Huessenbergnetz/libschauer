/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "job_p.h"
#include "logging.h"
#include "abstractnamfactory.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QReadWriteLock>
#include <QGlobalStatic>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
#include <QTimer>
#endif

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(schCore, "schauer.core")
#else
Q_LOGGING_CATEGORY(schCore, "schauer.core", QtInfoMsg)
#endif

using namespace Schauer;

class DefaultValues
{
public:
    mutable QReadWriteLock lock;

    AbstractConfiguration *configuration() const
    {
        return m_configuration;
    }

    void setConfiguration(AbstractConfiguration *config)
    {
        m_configuration = config;
    }

    AbstractNamFactory *namFactory() const
    {
        return m_namFactory;
    }

    void setNamFactory(AbstractNamFactory *factory)
    {
        m_namFactory = factory;
    }

private:
    AbstractConfiguration *m_configuration = nullptr;
    AbstractNamFactory *m_namFactory = nullptr;
};
Q_GLOBAL_STATIC(DefaultValues, defVals)

AbstractConfiguration *Schauer::defaultConfiguration()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractConfiguration *config = defs->configuration();
    defs->lock.unlock();

    return config;
}

void Schauer::setDefaultConfiguration(AbstractConfiguration *configuration)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(schCore) << "Setting defaultConfiguration to" << configuration;
    defs->setConfiguration(configuration);
}

AbstractNamFactory *Schauer::networkAccessManagerFactory()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNamFactory *namf = defs->namFactory();
    defs->lock.unlock();

    return namf;
}

void Schauer::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(schCore) << "Setting networkAccessManagerFactory to" << factory;
    defs->setNamFactory(factory);
}

JobPrivate::JobPrivate(Job *q)
    : q_ptr(q)
{

}

JobPrivate::~JobPrivate() = default;

void JobPrivate::handleSsslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    if (configuration->ignoreSslErrors()) {
        if (schCore().isWarningEnabled()) {
            for (const QSslError &e : errors) {
                qCWarning(schCore) << "Ignoring SSL error:" << e.errorString();
            }
        }
        reply->ignoreSslErrors();
    } else {
        Q_Q(Job);
        q->setError(SslError);
        if (!errors.empty()) {
            q->setErrorText(errors.first().errorString());
        } else {
            //: Error mesage
            //% "Can not perfrom API request. An unknown SSL error has occured."
            q->setErrorText(qtTrId("libschauer-error-unknown-ssl"));
        }
        qCCritical(schCore) << "SSL error:" << errors.first().errorString();
        reply->abort();
    }
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
void JobPrivate::requestTimedOut()
{
    Q_Q(Job);
    QNetworkReply *nr = reply;
    reply = nullptr;
    delete nr;

    q->setError(RequestTimedOut);
    q->setErrorText(QString::number(requestTimeout));
    q->emitResult();
}
#endif

void JobPrivate::requestFinished()
{
    Q_Q(Job);

    //: Job info message to display state information
    //% "Checking reply"
    Q_EMIT q->infoMessage(q, qtTrId("libschauer-info-msg-req-checking"));
    qCDebug(schCore) << "Request finished, checking reply.";
    qCDebug(schCore) << "HTTP status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    const QByteArray replyData = reply->readAll();

    qCDebug(schCore) << "Reply data:" << replyData;

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(timeoutTimer && timeoutTimer->isActive())) {
        qCDebug(schCore) << "Stopping request timeout timer with" << timeoutTimer->remainingTime() << "ms left";
        timeoutTimer->stop();
    }
#endif

    if (Q_LIKELY(reply->error() == QNetworkReply::NoError)) {
        if (Q_LIKELY(checkOutput(replyData))) {
            Q_EMIT q->succeeded(jsonResult);
        } else {
            qCDebug(schCore) << "Error code:" << q->error();
            Q_EMIT q->failed(q->error(), q->errorString());
        }
    } else {
        extractError(replyData);
        if (Q_UNLIKELY(q->error() == SJob::NoError)) {
            Q_EMIT q->succeeded(jsonResult);
        } else {
            Q_EMIT q->failed(q->error(), q->errorString());
        }
    }

    reply->deleteLater();
    reply = nullptr;

    q->emitResult();
}

void JobPrivate::extractError(const QByteArray &data)
{
    Q_Q(Job);
    if (q->error() == SJob::NoError) {

        q->setError(APIError);

        QJsonParseError jsonError;
        jsonResult = QJsonDocument::fromJson(data, &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            //: Error message
            //% "An error occured while performing the API request but the returned JSON error data is not parseable."
            q->setErrorText(qtTrId("libschauer-err-invalid-error-json-data"));
            qCCritical(schCore) << "Invalid JSON data in error reply at offset" << jsonError.offset << ":" <<jsonError.errorString();
            return;
        }

        if (!jsonResult.isObject()) {
            //: Error message
            //% "An error occured while performing the API request but the returned JSON error data does not conain a JSON object."
            q->setErrorText(qtTrId("libschauer-err-invalid-error-json-type"));
            qCCritical(schCore) << "Unexpected JSON type in error reply. Expected an object but got an array.";
            return;
        }

        const QString message = jsonResult.object().value(QStringLiteral("message")).toString();
        if (Q_UNLIKELY(message.isEmpty())) {
            //% "An error occured while performing the API request but the returned error message is empty."
            q->setErrorText(qtTrId("libschauer-err-empty-error-message"));
            qCCritical(schCore) << "The error message returned by the Docker API is empty.";
            return;
        }

        qCCritical(schCore) << "The following error occured while performing the API request:" << message;
        q->setErrorText(message);
    }
}

void JobPrivate::emitError(int errorCode, const QString &errorText)
{
    Q_Q(Job);
    q->setError(errorCode);
    q->setErrorText(errorText);
    q->emitResult();
    Q_EMIT q->failed(errorCode, q->errorString());
}

QString JobPrivate::buildUrlPath() const
{
    return QStringLiteral("/v1.40");
}

QUrlQuery JobPrivate::buildUrlQuery() const
{
    QUrlQuery query;
    return query;
}

QMap<QByteArray,QByteArray> JobPrivate::buildRequestHeaders() const
{
    QMap<QByteArray,QByteArray> headers;
    return headers;
}

std::pair<QByteArray,QByteArray> JobPrivate::buildPayload() const
{
    return std::make_pair(QByteArray(), QByteArray());
}

bool JobPrivate::checkInput()
{
    if (Q_UNLIKELY(configuration->host().isEmpty())) {
        emitError(MissingHost);
        qCCritical(schCore) << "Can not send request: missing host.";
        return false;
    }

    return true;
}

bool JobPrivate::checkOutput(const QByteArray &data)
{
    Q_Q(Job);

    if (expectedContentType != ExpectedContentType::Empty && data.isEmpty()) {
        q->setError(EmptyReply);
        qCCritical(schCore) << "Invalid reply: content expected, but reply is empty.";
        return false;
    }

    if (expectedContentType == ExpectedContentType::JsonArray || expectedContentType == ExpectedContentType::JsonObject) {
        QJsonParseError jsonError;
        jsonResult = QJsonDocument::fromJson(data, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            q->setError(JsonParseError);
            q->setErrorText(jsonError.errorString());
            qCCritical(schCore) << "Invalid JSON data in reply at offset" << jsonError.offset << ":" << jsonError.errorString();
            return false;
        }
    }

    if ((expectedContentType == ExpectedContentType::JsonArray || expectedContentType == ExpectedContentType::JsonObject) && (jsonResult.isNull() || jsonResult.isEmpty())) {
        q->setError(EmptyJson);
        qCCritical(schCore) << "Invalid reply: content expected, but reply is empty.";
        return false;
    }

    if (expectedContentType == ExpectedContentType::JsonArray && !jsonResult.isArray()) {
        q->setError(WrongOutputType);
        qCCritical(schCore) << "Invalid reply: JSON array expected, but got something different.";
        return false;
    }

    if (expectedContentType == ExpectedContentType::JsonObject && !jsonResult.isObject()) {
        q->setError(WrongOutputType);
        qCCritical(schCore) << "Invalid reply: JSON object expected, but got something different.";
        return false;
    }

    return true;
}

void JobPrivate::emitDescription()
{

}

Job::Job(QObject *parent)
    : SJob(parent), s_ptr(new JobPrivate(this))
{

}

Job::Job(JobPrivate &dd, QObject *parent)
    : SJob(parent), s_ptr(&dd)
{

}

Job::~Job() = default;

void Job::sendRequest()
{
    Q_D(Job);

    d->emitDescription();

    //: Job info message to display state information
    //% "Setting up request"
    Q_EMIT infoMessage(this, qtTrId("libschauer-info-msg-req-setup"));
    qCDebug(schCore) << "Setting up network request.";

    if (!d->configuration) {
        d->configuration = Schauer::defaultConfiguration();
        if (d->configuration) {
            qCDebug(schCore) << "Using default configuration" << d->configuration;
            Q_EMIT configurationChanged(d->configuration);
        } else {
            d->emitError(MissingConfig);
            qCCritical(schCore) << "Can not send request: missing configuration.";
            return;
        }
    }

    if (Q_UNLIKELY(!d->checkInput())) {
        return;
    }

    QUrl url;
    if (d->configuration->useSsl()) {
        url.setScheme(QStringLiteral("https"));
    } else {
        url.setScheme(QStringLiteral("http"));
    }

    url.setHost(d->configuration->host());
    url.setPort(d->configuration->port());
    url.setPath(d->buildUrlPath());
    url.setQuery(d->buildUrlQuery());

    if (Q_UNLIKELY(!url.isValid()))  {
        d->emitError(InvalidRequestUrl, url.toString());
        return;
    }

    if (!d->nam) {
        auto namf = Schauer::networkAccessManagerFactory();
        if (namf) {
            d->nam = namf->create(this);
            qCDebug(schCore) << "Using" << d->nam << "created by NetworkAccessManagerFactory" << namf;
        } else {
            d->nam = new QNetworkAccessManager(this);
            qCDebug(schCore) << "Using default created" << d->nam;
        }

        connect(d->nam, &QNetworkAccessManager::sslErrors, this, [d](QNetworkReply *reply, const QList<QSslError> &errors){
            d->handleSsslErrors(reply, errors);
        });
    }

    QNetworkRequest nr(url);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(d->requestTimeout > 0)) {
        nr.setTransferTimeout(static_cast<int>(d->requestTimeout) * 1000);
    }
#endif

    switch (d->expectedContentType) {
    case ExpectedContentType::JsonObject:
    case ExpectedContentType::JsonArray:
        nr.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));
        break;
    case ExpectedContentType::Invalid:
        Q_ASSERT_X(false, "sending request", "invalid expected content type");
        break;
    default:
        break;
    }

    const QMap<QByteArray, QByteArray> reqHeaders = d->buildRequestHeaders();
    if (!reqHeaders.empty()) {
        auto i = reqHeaders.constBegin();
        while (i != reqHeaders.constEnd()) {
            nr.setRawHeader(i.key(), i.value());
            ++i;
        }
    }

    const auto payload = d->buildPayload();

    if (!payload.second.isEmpty()) {
        nr.setRawHeader(QByteArrayLiteral("Content-Type"), payload.second);
    }

    if (schCore().isDebugEnabled()) {
        QString opName;
        switch(d->namOperation) {
        case NetworkOperation::Head:
            opName = QStringLiteral("HEAD");
            break;
        case NetworkOperation::Post:
            opName = QStringLiteral("POST");
            break;
        case NetworkOperation::Put:
            opName = QStringLiteral("PUT");
            break;
        case NetworkOperation::Delete:
            opName = QStringLiteral("DELETE");
            break;
        case NetworkOperation::Get:
            opName = QStringLiteral("GET");
            break;
        default:
            Q_ASSERT_X(false, "sending request", "invalid network operation");
            break;
        }
        qCDebug(schCore) << "Start performing" << opName << "network operation.";
        qCDebug(schCore) << "API URL:" << url;
        const auto rhl = nr.rawHeaderList();
        for (const QByteArray &h : rhl) {
            if (h == QByteArrayLiteral("X-Registry-Auth")) {
                qCDebug(schCore, "%s: **************", h.constData());
            } else {
                qCDebug(schCore, "%s: %s", h.constData(), nr.rawHeader(h).constData());
            }
        }
        if (!payload.first.isEmpty()) {
            qCDebug(schCore) << "Payload:" << payload.first;
        }
    }

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(d->requestTimeout > 0)) {
        if (!d->timeoutTimer) {
            d->timeoutTimer = new QTimer(this);
            d->timeoutTimer->setSingleShot(true);
            d->timeoutTimer->setTimerType(Qt::VeryCoarseTimer);
            connect(d->timeoutTimer, &QTimer::timeout, this, [d](){
                d->requestTimedOut();
            });
        }
        d->timeoutTimer->start(static_cast<int>(d->requestTimeout) * 1000);
        qCDebug(schCore) << "Started request timeout timer with" << d->requestTimeout << "seconds.";
    }
#endif

    //: Job info message to display state information
    //% "Sending request"
    Q_EMIT infoMessage(this, qtTrId("libschauer-info-msg-req-send"));
    qCDebug(schCore) << "Sending network request.";

    switch(d->namOperation) {
    case NetworkOperation::Head:
        d->reply = d->nam->head(nr);
        break;
    case NetworkOperation::Post:
        d->reply = d->nam->post(nr, payload.first);
        break;
    case NetworkOperation::Put:
        d->reply = d->nam->put(nr, payload.first);
        break;
    case NetworkOperation::Delete:
        d->reply = d->nam->deleteResource(nr);
        break;
    case NetworkOperation::Get:
        d->reply = d->nam->get(nr);
        break;
    default:
        Q_ASSERT_X(false, "sending request", "invalid network operation");
        break;
    }

    connect(d->reply, &QNetworkReply::finished, this, [d](){
        d->requestFinished();
    });
}

AbstractConfiguration* Job::configuration() const
{
    Q_D(const Job);
    return d->configuration;
}

void Job::setConfiguration(AbstractConfiguration *configuration)
{
    Q_D(Job);
    if (configuration != d->configuration) {
        qCDebug(schCore) << "Changing configuration from" << d->configuration << "to" << configuration;
        d->configuration = configuration;
        Q_EMIT configurationChanged(d->configuration);
    }
}

QString Job::errorString() const
{
    switch (error()) {
    case MissingConfig:
        //: Error message
        //% "No configuration set."
        return qtTrId("libschauer-error-missing-config");
    case MissingHost:
        //: Error message
        //% "Missing remote host name."
        return qtTrId("libschauer-error-missing-host");
    case MissingUser:
        //: Error message
        //% "Missing username."
        return qtTrId("libschauer-error-missing-username");
    case MissingPassword:
        //: Error message
        //% "Missing password."
        return qtTrId("libschauer-error-missing-password");
    case AuthNFailed:
        //: Error message
        //% "Authentication failed at the remote server, please check your username and password."
        return qtTrId("libschauer-error-authn-failed");
    case AuthZFailed:
        //: Error message
        //% "Authorization failed, you are not allowed to perform this request."
        return qtTrId("libschauer-error-authz-failed");
    case InvalidRequestUrl:
        //: Error message, %1 will be the invalid URL string.
        //% "The URL (%1) generated to perform the request is not valid, please check your input values."
        return qtTrId("libschauer-error-invalid-req-url").arg(errorText());
    case RequestTimedOut:
        //: Error message, %1 will be the request timeout in seconds
        //% "The request timed out after %1 seconds."
        return qtTrId("libschauer-error-request-timeout").arg(errorText());
    case JsonParseError:
        //: Error message, %1 will be the JSON parse error string.
        //% "Failed to parse the received JSON data: %1"
        return qtTrId("libschauer-error-json-parse").arg(errorText());
    case SslError:
    case NetworkError:
    case APIError:
        return errorText();
    case EmptyJson:
    case EmptyReply:
        //: Error message
        //% "Unexpected empty reply data."
        return qtTrId("libschauer-error-empty-json");
    case WrongOutputType:
        //: Error message
        //% "Unexpected JSON type in received data."
        return qtTrId("libschauer-error-invalid-output-type");
    case InvalidInput:
        return errorText();
    default:
        //: Error message
        //% "Sorry, but unfortunately an unknown error has occurred."
        return qtTrId("libschauer-error-unknown");
    }
}

QJsonDocument Job::replyData() const
{
    Q_D(const Job);
    return d->jsonResult;
}

#include "moc_job.cpp"
