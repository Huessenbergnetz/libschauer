/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "createcontainerjob_p.h"
#include <QRegularExpression>
#include <QTimer>
#include <utility>
#include "logging.h"

using namespace Schauer;

CreateContainerJobPrivate::CreateContainerJobPrivate(CreateContainerJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Post;
    expectedContentType = ExpectedContentType::JsonObject;
    requiresAuth = false;
}

CreateContainerJobPrivate::~CreateContainerJobPrivate() = default;

QString CreateContainerJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/containers/create");
    return path;
}

QUrlQuery CreateContainerJobPrivate::buildUrlQuery() const
{
    QUrlQuery uq = JobPrivate::buildUrlQuery();
    if (!name.isEmpty()) {
        uq.addQueryItem(QStringLiteral("name"), name);
    }
    return uq;
}

std::pair<QByteArray, QByteArray> CreateContainerJobPrivate::buildPayload() const
{
    const QJsonObject ccObj = QJsonObject::fromVariantHash(containerConfig);
    const QJsonDocument ccDoc(ccObj);

    return std::make_pair(ccDoc.toJson(QJsonDocument::Compact), QByteArrayLiteral("application/json"));
}

void CreateContainerJobPrivate::emitDescription()
{
    Q_Q(CreateContainerJob);

    QString _title;

    if (name.isEmpty()) {
        //: Job description title
        //% "Creating new container"
        _title = qtTrId("libschauer-job-desc-create-container-title");
    } else {
        //: Job description title, %1 will be replaced by the container name
        //% "Creating new container %1"
        _title = qtTrId("libschauer-job-desc-create-container-title-with-name").arg(name);
    }

    //: Job description field name
    //% "Image"
    const QString f1Name = qtTrId("libschauer-job-desc-create-container-field1");

    Q_EMIT q->description(q, _title, qMakePair(f1Name, name));
}

bool CreateContainerJobPrivate::checkInput()
{
    if (!JobPrivate::checkInput()) {
        return false;
    }

    if (!name.isEmpty()) {
        const QString reStr = QStringLiteral("^/?[a-zA-Z0-9][a-zA-Z0-9_.-]+$");
        static QRegularExpression re(reStr);
        if (!name.contains(re)) {
            //: Error message if the container name is not valid, %1 will b replaced by the regular expression
            //% "The name for the container is not valid. It has to match the following regular expression: %1"
            emitError(InvalidInput, qtTrId("libschauer-error-invalid-input-container-name").arg(reStr));
            qCCritical(schCore) << "Invalid container name" << name << "does not match this required regular expression:" << reStr;
            return false;
        }
    }

    if (containerConfig.value(QStringLiteral("Image")).toString().isEmpty()) {
        //: Error message if the image name is missing when trying to create a container
        //% "The name of the image from which the container is to be created is missing."
        emitError(InvalidInput, qtTrId("libschauer-error-invalid-input-image-name"));
        qCCritical(schCore) << "Missing image when trying to create container" << name;
        return false;
    }

    return true;
}

CreateContainerJob::CreateContainerJob(QObject *parent)
    : Job(* new CreateContainerJobPrivate(this), parent)
{

}

CreateContainerJob::~CreateContainerJob() = default;

void CreateContainerJob::start()
{
    QTimer::singleShot(0, this, &CreateContainerJob::sendRequest);
}

QString CreateContainerJob::name() const
{
    Q_D(const CreateContainerJob);
    return d->name;
}

void CreateContainerJob::setName(const QString &name)
{
    Q_D(CreateContainerJob);
    if (d->name != name) {
        qCDebug(schCore) << "Changing \"name\" from" << d->name << "to" << name;
        d->name = name;
        Q_EMIT nameChanged(this->name());
    }
}

QVariantHash CreateContainerJob::containerConfig() const
{
    Q_D(const CreateContainerJob);
    return d->containerConfig;
}

void CreateContainerJob::setContainerConfig(const QVariantHash &containerConfig)
{
    Q_D(CreateContainerJob);
    if (d->containerConfig != containerConfig) {
        qCDebug(schCore) << "Changing \"containerConfig\" from" << d->containerConfig << "to" << containerConfig;
        d->containerConfig = containerConfig;
        Q_EMIT containerConfigChanged(this->containerConfig());
    }
}

#include "moc_createcontainerjob.cpp"
