/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "removecontainerjob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Schauer;

RemoveContainerJobPrivate::RemoveContainerJobPrivate(RemoveContainerJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Delete;
    expectedContentType = ExpectedContentType::Empty;
    requiresAuth = false;
}

RemoveContainerJobPrivate::~RemoveContainerJobPrivate() = default;

QString RemoveContainerJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/containers/") + id;
    return path;
}

QUrlQuery RemoveContainerJobPrivate::buildUrlQuery() const
{
    QUrlQuery uq;
    if (removeAnonVolumes) {
        uq.addQueryItem(QStringLiteral("v"), QStringLiteral("true"));
    }
    if (force) {
        uq.addQueryItem(QStringLiteral("force"), QStringLiteral("true"));
    }
    if (removeLinks) {
        uq.addQueryItem(QStringLiteral("links"), QStringLiteral("true"));
    }
    return uq;
}

void RemoveContainerJobPrivate::emitDescription()
{
    Q_Q(RemoveContainerJob);

    //: Job description title
    //% "Remove container with ID %1"
    const QString _title = qtTrId("libschauer-job-desc-remove-container-title").arg(id);

    Q_EMIT q->description(q, _title);
}

bool RemoveContainerJobPrivate::checkInput()
{
    if (!JobPrivate::checkInput()) {
        return false;
    }

    if (id.isEmpty()) {
        //: Error message if container id is missing when tryping to remove a container
        //% "Can n ot remove a container without a valid container ID."
        emitError(InvalidInput, qtTrId("libschauer-error-remove-container-missing-id"));
        qCCritical(schCore) << "Missing container ID when trying to remove a container.";
        return false;
    }

    return true;
}

RemoveContainerJob::RemoveContainerJob(QObject *parent)
    : Job(* new RemoveContainerJobPrivate(this), parent)
{

}

RemoveContainerJob::~RemoveContainerJob() = default;

void RemoveContainerJob::start()
{
    QTimer::singleShot(0, this, &RemoveContainerJob::sendRequest);
}

QString RemoveContainerJob::id() const
{
    Q_D(const RemoveContainerJob);
    return d->id;
}

void RemoveContainerJob::setId(const QString &id)
{
    Q_D(RemoveContainerJob);
    if (d->id != id) {
        qCDebug(schCore) << "Changing \"id\" from" << d->id << "to" << id;
        d->id = id;
        Q_EMIT idChanged(this->id());
    }
}

bool RemoveContainerJob::removeAnonVolumes() const
{
    Q_D(const RemoveContainerJob);
    return d->removeAnonVolumes;
}

void RemoveContainerJob::setRemoveAnonVolumes(bool removeAnonVolumes)
{
    Q_D(RemoveContainerJob);
    if (d->removeAnonVolumes != removeAnonVolumes) {
        qCDebug(schCore) << "Changing \"removeAnonVolumes\" from" << d->removeAnonVolumes << "to" << removeAnonVolumes;
        d->removeAnonVolumes = removeAnonVolumes;
        Q_EMIT removeAnonVolumesChanged(this->removeAnonVolumes());
    }
}

bool RemoveContainerJob::force() const
{
    Q_D(const RemoveContainerJob);
    return d->force;
}

void RemoveContainerJob::setForce(bool force)
{
    Q_D(RemoveContainerJob);
    if (d->force != force) {
        qCDebug(schCore) << "Changing \"force\" from" << d->force << "to" << force;
        d->force = force;
        Q_EMIT forceChanged(this->force());
    }
}

bool RemoveContainerJob::removeLinks() const
{
    Q_D(const RemoveContainerJob);
    return d->removeLinks;
}

void RemoveContainerJob::setRemoveLinks(bool removeLinks)
{
    Q_D(RemoveContainerJob);
    if (d->removeLinks != removeLinks) {
        qCDebug(schCore) << "Changing \"removeLinks\" from" << d->removeLinks << "to" << removeLinks;
        d->removeLinks = removeLinks;
        Q_EMIT removeLinksChanged(this->removeLinks());
    }
}

#include "moc_removecontainerjob.cpp"
