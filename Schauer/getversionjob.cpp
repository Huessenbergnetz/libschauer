/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getversionjob_p.h"
#include <QTimer>

using namespace Schauer;

GetVersionJobPrivate::GetVersionJobPrivate(GetVersionJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Get;
    expectedContentType = ExpectedContentType::JsonObject;
    requiresAuth = false;
}

GetVersionJobPrivate::~GetVersionJobPrivate() = default;

QString GetVersionJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/version");
    return path;
}

void GetVersionJobPrivate::emitDescription()
{
    Q_Q(GetVersionJob);

    //: Job title
    //% "Requesting version information"
    const QString _title = qtTrId("libschauer-job-desc-get-version-title");

    Q_EMIT q->description(q, _title);
}

GetVersionJob::GetVersionJob(QObject *parent)
    : Job(* new GetVersionJobPrivate(this), parent)
{

}

GetVersionJob::~GetVersionJob() = default;

void GetVersionJob::start()
{
    QTimer::singleShot(0, this, &GetVersionJob::sendRequest);
}

#include "moc_getversionjob.cpp"
