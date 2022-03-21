/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "startcontainerjob_p.h"
#include "logging.h"
#include <QTimer>
#include <QRegularExpression>

using namespace Schauer;

StartContainerJobPrivate::StartContainerJobPrivate(StartContainerJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Post;
    expectedContentType = ExpectedContentType::Empty;
    requiresAuth = false;
}

StartContainerJobPrivate::~StartContainerJobPrivate() = default;

QString StartContainerJobPrivate::buildUrlPath() const
{
    const QString _id = id.startsWith(QLatin1Char('/')) ? id.mid(1) : id;
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/containers/") + _id + QLatin1String("/start");
    return path;
}

QUrlQuery StartContainerJobPrivate::buildUrlQuery() const
{
    QUrlQuery uq = JobPrivate::buildUrlQuery();
    if (!detachKeys.isEmpty()) {
        uq.addQueryItem(QStringLiteral("detachKeys"), detachKeys);
    }
    return uq;
}

void StartContainerJobPrivate::emitDescription()
{
    Q_Q(StartContainerJob);

    //: Job description title
    //% "Starting container with ID %1"
    const QString _title = qtTrId("libschauer-job-desc-start-container-title").arg(id);

    Q_EMIT q->description(q, _title);
}

bool StartContainerJobPrivate::checkInput()
{
    if (!JobPrivate::checkInput()) {
        return false;
    }

    if (id.isEmpty()) {
        //: Error message if container id is missing when trying to start a container
        //% "Can not start a container without a valid container ID."
        emitError(InvalidInput, qtTrId("libschauer-error-start-container-missing-id"));
        qCCritical(schCore) << "Missing container ID when trying to start a container";
        return false;
    }

    if (!detachKeys.isEmpty()) {
        const QString reStr = QStringLiteral("^[a-zA-Z]$|^ctrl-[a-z@\\^\\[,_]$");
        static QRegularExpression re(reStr);
        if (!detachKeys.contains(re)) {
            //: Error message if container detachKeys property is malformed
            //% "Invalid “detachKeys” parameter. Format is a single charachter a-Z or “ctrl-<value>” where <value> is one of: a-z, @, ^, [, _ or ,."
            emitError(InvalidInput, qtTrId("libschauer-error-start-container-invalid-detachkeys"));
            return false;
        }
    }

    return true;
}

StartContainerJob::StartContainerJob(QObject *parent)
    : Job(* new StartContainerJobPrivate(this), parent)
{

}

StartContainerJob::~StartContainerJob() = default;

void StartContainerJob::start()
{
    QTimer::singleShot(0, this, &StartContainerJob::sendRequest);
}

QString StartContainerJob::id() const
{
    Q_D(const StartContainerJob);
    return d->id;
}

void StartContainerJob::setId(const QString &id)
{
    Q_D(StartContainerJob);
    if (d->id != id) {
        qCDebug(schCore) << "Changing \"id\" from" << d->id << "to" << id;
        d->id = id;
        Q_EMIT idChanged(this->id());
    }
}

QString StartContainerJob::detachKeys() const
{
    Q_D(const StartContainerJob);
    return d->detachKeys;
}

void StartContainerJob::setDetachKeys(const QString &detachKeys)
{
    Q_D(StartContainerJob);
    if (d->detachKeys != detachKeys) {
        qCDebug(schCore) << "Changing \"detachKeys\" from" << d->detachKeys << "to" << detachKeys;
        d->detachKeys = detachKeys;
        Q_EMIT detachKeysChanged(this->detachKeys());
    }
}

#include "moc_startcontainerjob.cpp"
