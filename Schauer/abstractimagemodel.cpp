/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractimagemodel_p.h"
#include "listimagesjob.h"
#include "logging.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

using namespace Schauer;

AbstractImageModelPrivate::AbstractImageModelPrivate(AbstractImageModel *q)
    : AbstractBaseModelPrivate(q)
{

}

AbstractImageModelPrivate::~AbstractImageModelPrivate() = default;

void AbstractImageModelPrivate::setupJob()
{
    Q_Q(AbstractImageModel);
    auto _job = new ListImagesJob(q);
    _job->setShowAll(showAll);
    _job->setShowDigests(showDigests);
    job = _job;
}

bool AbstractImageModelPrivate::loadFromJson(const QJsonDocument &json)
{
    Q_Q(AbstractImageModel);

    const QJsonArray imgs = json.array();

    q->beginInsertRows(QModelIndex(), images.size(), images.size() + imgs.size() - 1);

    for (const QJsonValue &img : imgs) {
        const QJsonObject o = img.toObject();
        const QString id = o.value(QStringLiteral("Id")).toString();
        const QString parentId = o.value(QStringLiteral("ParentId")).toString();
        const QStringList repoTags = AbstractBaseModelPrivate::jsonArrayToStringList(o.value(QStringLiteral("RepoTags")));
        const QStringList repoDigests = AbstractBaseModelPrivate::jsonArrayToStringList(o.value(QStringLiteral("RepoDigests")));
        const QDateTime created = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(o.value(QStringLiteral("Created")).toDouble()), Qt::UTC);
        const qint64 size = static_cast<qint64>(o.value(QStringLiteral("Size")).toDouble());
        const qint64 virtualSize = static_cast<qint64>(o.value(QStringLiteral("VirtualSize")).toDouble());
        const qint64 sharedSize = static_cast<qint64>(o.value(QStringLiteral("SharedSize")).toDouble());
        const QMap<QString,QString> labels = AbstractBaseModelPrivate::jsonObjectToStringMap(o.value(QStringLiteral("Labels")));
        const int containers = o.value(QStringLiteral("Containers")).toInt();

        images.emplace_back(id, parentId, repoTags, repoDigests, created, size, virtualSize, sharedSize, labels, containers);
    }

    q->endInsertRows();

    Q_EMIT q->loaded();

    setIsLoading(false);

    return true;
}

AbstractImageModel::AbstractImageModel(QObject *parent)
    : AbstractBaseModel(* new AbstractImageModelPrivate(this), parent)
{

}

AbstractImageModel::AbstractImageModel(AbstractImageModelPrivate &dd, QObject *parent)
    : AbstractBaseModel(dd, parent)
{

}

AbstractImageModel::~AbstractImageModel() = default;

int AbstractImageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_D(const AbstractImageModel);
    return d->images.size();
}

bool AbstractImageModel::showAll() const
{
    Q_D(const AbstractImageModel);
    return d->showAll;
}

void AbstractImageModel::setShowAll(bool showAll)
{
    Q_D(AbstractImageModel);
    if (d->showAll != showAll) {
        qCDebug(schCore) << "Changing \"schowAll\" from" << d->showAll << "to" << showAll;
        d->showAll = showAll;
        Q_EMIT showAllChanged(this->showAll());
    }
}

bool AbstractImageModel::showDigests() const
{
    Q_D(const AbstractImageModel);
    return d->showDigests;
}

void AbstractImageModel::setShowDigests(bool showDigests)
{
    Q_D(AbstractImageModel);
    if (d->showDigests != showDigests) {
        qCDebug(schCore) << "Changing \"showDigests\" from" << d->showDigests << "to" << showDigests;
        d->showDigests = showDigests;
        Q_EMIT showDigestsChanged(this->showDigests());
    }
}

bool AbstractImageModel::containsRepoTag(const QString &repo, const QString &tag) const
{
    return containsRepoTag(QLatin1String(repo.toLatin1()), QLatin1String(tag.toLatin1()));
}

bool AbstractImageModel::containsRepoTag(QLatin1String repo, QLatin1String tag) const
{
    Q_D(const AbstractImageModel);
    if (d->images.empty() || repo.isEmpty()) {
        return false;
    }
    if (tag.isEmpty()) {
        for (const ImageModelItem &item : static_cast<const std::vector<ImageModelItem>&>(d->images)) {
            for (const QString &repoTag : static_cast<const QStringList &>(item.repoTags)) {
                const int colonIdx = repoTag.indexOf(QLatin1Char(':'));
                if (Q_LIKELY(colonIdx > -1)) {
                    if (repoTag.leftRef(colonIdx).compare(repo) == 0) {
                        return true;
                    }
                }
            }
        }
    } else {
        for (const ImageModelItem &item : static_cast<const std::vector<ImageModelItem>&>(d->images)) {
            for (const QString &repoTag : static_cast<const QStringList &>(item.repoTags)) {
                const int colonIdx = repoTag.indexOf(QLatin1Char(':'));
                if (Q_LIKELY(colonIdx > -1)) {
                    if (repoTag.leftRef(colonIdx).compare(repo) == 0 && repoTag.midRef(colonIdx + 1).compare(tag) == 0) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void AbstractImageModel::clear()
{
    Q_D(AbstractImageModel);
    if (!d->images.empty()) {

        beginRemoveRows(QModelIndex(), 0, d->images.size() - 1);

        d->images.clear();

        endRemoveRows();
    }
}

#include "moc_abstractimagemodel.cpp"
