/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "listimagesjob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Schauer;

ListImagesJobPrivate::ListImagesJobPrivate(ListImagesJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Get;
    expectedContentType = ExpectedContentType::JsonArray;
    requiresAuth = false;
}

ListImagesJobPrivate::~ListImagesJobPrivate() = default;

QString ListImagesJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/images/json");
    return path;
}

QUrlQuery ListImagesJobPrivate::buildUrlQuery() const
{
    QUrlQuery uq = JobPrivate::buildUrlQuery();
    if (showAll) {
        uq.addQueryItem(QStringLiteral("all"), QStringLiteral("true"));
    }
    if (showDigests) {
        uq.addQueryItem(QStringLiteral("digests"), QStringLiteral("true"));
    }
    return uq;
}

void ListImagesJobPrivate::emitDescription()
{
    Q_Q(ListImagesJob);

    //: Job title
    //% "Requesting list of images"
    const QString _title = qtTrId("libschauer-job-desc-list-images-title");

    Q_EMIT q->description(q, _title);
}

ListImagesJob::ListImagesJob(QObject *parent)
    : Job(* new ListImagesJobPrivate(this), parent)
{

}

ListImagesJob::~ListImagesJob() = default;

void ListImagesJob::start()
{
    QTimer::singleShot(0, this, &ListImagesJob::sendRequest);
}

bool ListImagesJob::showAll() const
{
    Q_D(const ListImagesJob);
    return d->showAll;
}

void ListImagesJob::setShowAll(bool showAll)
{
    Q_D(ListImagesJob);
    if (d->showAll != showAll) {
        qCDebug(schCore) << "Changing \"showAll\" from" << d->showAll << "to" << showAll;
        d->showAll = showAll;
        Q_EMIT showAllChanged(this->showAll());
    }
}

bool ListImagesJob::showDigests() const
{
    Q_D(const ListImagesJob);
    return d->showDigests;
}

void ListImagesJob::setShowDigests(bool showDigests)
{
    Q_D(ListImagesJob);
    if (d->showDigests != showDigests) {
        qCDebug(schCore) << "Changing \"showDigests\" from" << d->showDigests << "to" << showDigests;
        d->showDigests = showDigests;
        Q_EMIT showDigestsChanged(this->showDigests());
    }
}

#include "moc_listimagesjob.cpp"
