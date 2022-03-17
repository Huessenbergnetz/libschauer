/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_JOB_P_H
#define SCHAUER_JOB_P_H

#include "job.h"
#include <QUrlQuery>
#include <QSslError>
#include <utility>

class QNetworkReply;
class QNetworkAccessManager;
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
class QTimer;
#endif

namespace Schauer {

enum class ExpectedContentType : qint8 {
    Invalid     = -1,
    Empty       = 0,
    JsonArray   = 1,
    JsonObject  = 2
};

enum class NetworkOperation : qint8 {
    Invalid = 0,
    Head    = 1,
    Get     = 2,
    Put     = 3,
    Post    = 4,
    Delete  = 5,
    Custom  = 6
};

class JobPrivate
{
public:
    JobPrivate(Job *q);
    virtual ~JobPrivate();

    QJsonDocument jsonResult;
    QNetworkAccessManager *nam = nullptr;
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    QTimer *timeoutTimer = nullptr;
#endif
    QNetworkReply *reply = nullptr;
    AbstractConfiguration *configuration = nullptr;
    NetworkOperation namOperation = NetworkOperation::Invalid;
    ExpectedContentType expectedContentType = ExpectedContentType::Invalid;
    int statusCode = 0;
    quint16 requestTimeout = 300;
    bool requiresAuth = true;

    void handleSsslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    void requestTimedOut();
#endif

    void requestFinished();

    void emitError(int errorCode, const QString &errorText = QString());

    virtual QString buildUrlPath() const;

    virtual QUrlQuery buildUrlQuery() const;

    virtual QMap<QByteArray, QByteArray> buildRequestHeaders() const;

    virtual std::pair<QByteArray, QByteArray> buildPayload() const;

    virtual bool checkInput();

    virtual bool checkOutput(const QByteArray &data);

    virtual void extractError(const QByteArray &data);

    virtual void emitDescription();

protected:
    Job *q_ptr = nullptr;

private:
    Q_DISABLE_COPY(JobPrivate)
    Q_DECLARE_PUBLIC(Job)
};

}

#endif // SCHAUER_JOB_P_H
