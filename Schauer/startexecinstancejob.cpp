/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "startexecinstancejob_p.h"
#include "logging.h"
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <utility>

using namespace Schauer;

StartExecInstanceJobPrivate::StartExecInstanceJobPrivate(StartExecInstanceJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Post;
    expectedContentType = ExpectedContentType::Empty;
    requiresAuth = false;
}

StartExecInstanceJobPrivate::~StartExecInstanceJobPrivate() = default;

QString StartExecInstanceJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/exec/") + id + QLatin1String("/start");
    return path;
}

std::pair<QByteArray,QByteArray> StartExecInstanceJobPrivate::buildPayload() const
{
    const QJsonObject body({
                               {QStringLiteral("Detach"), detach},
                               {QStringLiteral("Tty"), tty}
                           });
    const QJsonDocument bodyDoc(body);

    return std::make_pair(bodyDoc.toJson(QJsonDocument::Compact), QByteArrayLiteral("application/json"));
}

void StartExecInstanceJobPrivate::emitDescription()
{
    Q_Q(StartExecInstanceJob);

    //: Job description title
    //% "Starting execution instance."
    const QString _title = qtTrId("libschauer-job-desc-start-exec");

    // Job description field name
    //% "ID"
    const QString _f1Name = qtTrId("libschauer-job-desc-field1-name");

    Q_EMIT q->description(q, _title, qMakePair(_f1Name, id));
}

bool StartExecInstanceJobPrivate::checkInput()
{
    if (!JobPrivate::checkInput()) {
        return false;
    }

    if (id.isEmpty()) {
        //: Error when trying to start an exec instance
        //% "Missing execution instance ID to start."
        emitError(InvalidInput, qtTrId("libschauer-start-exec-instance-err-missing-id"));
        qCCritical(schCore) << "Missing execution instance ID to start.";
        return false;
    }

    return true;
}

StartExecInstanceJob::StartExecInstanceJob(QObject *parent)
    : Job(* new StartExecInstanceJobPrivate(this), parent)
{

}

StartExecInstanceJob::~StartExecInstanceJob() = default;

void StartExecInstanceJob::start()
{
    QTimer::singleShot(0, this, &StartExecInstanceJob::sendRequest);
}

QString StartExecInstanceJob::id() const
{
    Q_D(const StartExecInstanceJob);
    return d->id;
}

void StartExecInstanceJob::setId(const QString &id)
{
    Q_D(StartExecInstanceJob);
    if (d->id != id) {
        qCDebug(schCore) << "Changing \"id\" from" << d->id << "to" << id;
        d->id = id;
        Q_EMIT idChanged(this->id());
    }
}

bool StartExecInstanceJob::detach() const
{
    Q_D(const StartExecInstanceJob);
    return d->detach;
}

void StartExecInstanceJob::setDetach(bool detach)
{
    Q_D(StartExecInstanceJob);
    if (d->detach != detach) {
        qCDebug(schCore) << "Changing \"detach\" from" << d->detach << "to" << detach;
        d->detach = detach;
        Q_EMIT detachChanged(this->detach());
    }
}

bool StartExecInstanceJob::tty() const
{
    Q_D(const StartExecInstanceJob);
    return d->tty;
}

void StartExecInstanceJob::setTty(bool tty)
{
    Q_D(StartExecInstanceJob);
    if (d->tty != tty) {
        qCDebug(schCore) << "Changing \"tty\" from" << d->tty << "to" << tty;
        d->tty = tty;
        Q_EMIT ttyChanged(this->tty());
    }
}

#include "moc_startexecinstancejob.cpp"
