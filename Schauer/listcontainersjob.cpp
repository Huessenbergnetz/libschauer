/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "listcontainersjob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Schauer;

ListContainersJobPrivate::ListContainersJobPrivate(ListContainersJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Get;
    expectedContentType = ExpectedContentType::JsonArray;
    requiresAuth = false;
}

ListContainersJobPrivate::~ListContainersJobPrivate() = default;

QString ListContainersJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/containers/json");
    return path;
}

QUrlQuery ListContainersJobPrivate::buildUrlQuery() const
{
    QUrlQuery uq = JobPrivate::buildUrlQuery();
    if (showAll) {
        uq.addQueryItem(QStringLiteral("all"), QStringLiteral("true"));
    }
    if (limit > 0) {
        uq.addQueryItem(QStringLiteral("limit"), QString::number(limit));
    }
    if (showSize) {
        uq.addQueryItem(QStringLiteral("size"), QStringLiteral("true"));
    }
    return uq;
}

void ListContainersJobPrivate::emitDescription()
{
    Q_Q(ListContainersJob);

    //: Job title
    //% "Requesting list of containers"
    const QString _title = qtTrId("libschauer-job-desc-list-containers-title");

    Q_EMIT q->description(q, _title);
}

ListContainersJob::ListContainersJob(QObject *parent)
    : Job(* new ListContainersJobPrivate(this), parent)
{

}

ListContainersJob::~ListContainersJob() = default;

void ListContainersJob::start()
{
    QTimer::singleShot(0, this, &ListContainersJob::sendRequest);
}

int ListContainersJob::limit() const
{
    Q_D(const ListContainersJob);
    return d->limit;
}

void ListContainersJob::setLimit(int limit)
{
    Q_D(ListContainersJob);
    if (d->limit != limit) {
        qCDebug(schCore) << "Changing \"limit\" from" << d->limit << "to" << limit;
        d->limit = limit;
        Q_EMIT limitChanged(this->limit());
    }
}

bool ListContainersJob::showAll() const
{
    Q_D(const ListContainersJob);
    return d->showAll;
}

void ListContainersJob::setShowAll(bool showAll)
{
    Q_D(ListContainersJob);
    if (d->showAll != showAll) {
        qCDebug(schCore) << "Changing \"showAll\" from" << d->showAll << "to" << showAll;
        d->showAll = showAll;
        Q_EMIT showAllChanged(this->showAll());
    }
}

bool ListContainersJob::showSize() const
{
    Q_D(const ListContainersJob);
    return d->showSize;
}

void ListContainersJob::setShowSize(bool showSize)
{
    Q_D(ListContainersJob);
    if (d->showSize != showSize) {
        qCDebug(schCore) << "Changing \"showSize\" from" << d->showSize << "to" << showSize;
        d->showSize = showSize;
        Q_EMIT showSizeChanged(showSize);
    }
}

#include "moc_listcontainersjob.cpp"
