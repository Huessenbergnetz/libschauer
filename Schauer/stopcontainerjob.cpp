/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "stopcontainerjob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Schauer;

StopContainerJobPrivate::StopContainerJobPrivate(StopContainerJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Post;
    expectedContentType = ExpectedContentType::Empty;
    requiresAuth = false;
}

StopContainerJobPrivate::~StopContainerJobPrivate() = default;

QString StopContainerJobPrivate::buildUrlPath() const
{
    const QString _id = id.startsWith(QLatin1Char('/')) ? id.mid(1) : id;
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/containers/") + _id + QLatin1String("/stop");
    return path;
}

QUrlQuery StopContainerJobPrivate::buildUrlQuery() const
{
    QUrlQuery uq = JobPrivate::buildUrlQuery();
    if (timeout > 0) {
        uq.addQueryItem(QStringLiteral("t"), QString::number(timeout));
    }
    return uq;
}

void StopContainerJobPrivate::emitDescription()
{
    Q_Q(StopContainerJob);

    //: Job description title
    //% "Stopping container with ID %1"
    const QString _title = qtTrId("libschauer-job-desc-stop-container-title").arg(id);

    Q_EMIT q->description(q, _title);
}

bool StopContainerJobPrivate::checkInput()
{
    if (!JobPrivate::checkInput()) {
        return false;
    }

    if (id.isEmpty()) {
        //: Error message if container id is missing when trying to stop a container
        //% "Can not stop a container without a valid container ID."
        emitError(InvalidInput, qtTrId("libschauer-error-stop-container-missing-id"));
        qCCritical(schCore) << "Missing container ID when trying to stop a container";
        return false;
    }

    return true;
}

StopContainerJob::StopContainerJob(QObject *parent)
    : Job(* new StopContainerJobPrivate(this), parent)
{

}

StopContainerJob::~StopContainerJob() = default;

void StopContainerJob::start()
{
    QTimer::singleShot(0, this, &StopContainerJob::sendRequest);
}

QString StopContainerJob::id() const
{
    Q_D(const StopContainerJob);
    return d->id;
}

void StopContainerJob::setId(const QString &id)
{
    Q_D(StopContainerJob);
    if (d->id != id) {
        qCDebug(schCore) << "Changing \"id\" from" << d->id << "to" << id;
        d->id = id;
        Q_EMIT idChanged(this->id());
    }
}

int StopContainerJob::timeout() const
{
    Q_D(const StopContainerJob);
    return d->timeout;
}

void StopContainerJob::setTimeout(int timeout)
{
    Q_D(StopContainerJob);
    if (d->timeout != timeout) {
        qCDebug(schCore) << "Changing \"timeout\" from" << d->timeout << "to" << timeout;
        d->timeout = timeout;
        Q_EMIT timeoutChanged(this->timeout());
    }
}

#include "moc_stopcontainerjob.cpp"
