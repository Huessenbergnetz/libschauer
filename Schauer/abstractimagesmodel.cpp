/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractimagesmodel_p.h"
#include "listimagesjob.h"
#include "logging.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QVersionNumber>

using namespace Schauer;

AbstractImagesModelPrivate::AbstractImagesModelPrivate(AbstractImagesModel *q)
    : AbstractBaseModelPrivate(q)
{

}

AbstractImagesModelPrivate::~AbstractImagesModelPrivate() = default;

void AbstractImagesModelPrivate::setupJob()
{
    Q_Q(AbstractImagesModel);
    auto _job = new ListImagesJob(q);
    _job->setShowAll(showAll);
    _job->setShowDigests(showDigests);
    job = _job;
}

bool AbstractImagesModelPrivate::loadFromJson(const QJsonDocument &json)
{
    Q_Q(AbstractImagesModel);

    const QJsonArray imgs = json.array();

    q->beginInsertRows(QModelIndex(), images.size(), images.size() + imgs.size() - 1);

    for (const QJsonValue &img : imgs) {
        const QJsonObject o = img.toObject();
        const QString id = o.value(QStringLiteral("Id")).toString();
        const QString parentId = o.value(QStringLiteral("ParentId")).toString();

        QStringList repoTags;
        {
            const QJsonArray repoTagsArray = o.value(QStringLiteral("RepoTags")).toArray();
            repoTags.reserve(repoTagsArray.size());
            for (const QJsonValue &repoTag : repoTagsArray) {
                repoTags << repoTag.toString();
            }
        }

        QStringList repoDigests;
        {
            const QJsonArray repoDigestsArray = o.value(QStringLiteral("RepoDigests")).toArray();
            repoDigests.reserve(repoDigestsArray.size());
            for (const QJsonValue &repoDigest : repoDigestsArray) {
                repoDigests << repoDigest.toString();
            }
        }

        const QDateTime created = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(o.value(QStringLiteral("Created")).toDouble()), Qt::UTC);
        const qint64 size = static_cast<qint64>(o.value(QStringLiteral("Size")).toDouble());
        const qint64 virtualSize = static_cast<qint64>(o.value(QStringLiteral("VirtualSize")).toDouble());
        const qint64 sharedSize = static_cast<qint64>(o.value(QStringLiteral("SharedSize")).toDouble());

        QMap<QString,QString> labels;
        {
            const QJsonObject labelsObject = o.value(QStringLiteral("Labels")).toObject();
            if (!labelsObject.empty()) {
                const QStringList labelsKeys = labelsObject.keys();
                for (const QString &labelsKey : labelsKeys) {
                    labels.insert(labelsKey, labelsObject.value(labelsKey).toString());
                }
            }
        }

        const int containers = o.value(QStringLiteral("Containers")).toInt();

        images.emplace_back(id, parentId, repoTags, repoDigests, created, size, virtualSize, sharedSize, labels, containers);
    }

    q->endInsertRows();

    Q_EMIT q->loaded();

    setIsLoading(false);

    return true;
}

AbstractImagesModel::AbstractImagesModel(QObject *parent)
    : AbstractBaseModel(* new AbstractImagesModelPrivate(this), parent)
{

}

AbstractImagesModel::AbstractImagesModel(AbstractImagesModelPrivate &dd, QObject *parent)
    : AbstractBaseModel(dd, parent)
{

}

AbstractImagesModel::~AbstractImagesModel() = default;

int AbstractImagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_D(const AbstractImagesModel);
    return d->images.size();
}

bool AbstractImagesModel::showAll() const
{
    Q_D(const AbstractImagesModel);
    return d->showAll;
}

void AbstractImagesModel::setShowAll(bool showAll)
{
    Q_D(AbstractImagesModel);
    if (d->showAll != showAll) {
        qCDebug(schCore) << "Changing \"schowAll\" from" << d->showAll << "to" << showAll;
        d->showAll = showAll;
        Q_EMIT showAllChanged(this->showAll());
    }
}

bool AbstractImagesModel::showDigests() const
{
    Q_D(const AbstractImagesModel);
    return d->showDigests;
}

void AbstractImagesModel::setShowDigests(bool showDigests)
{
    Q_D(AbstractImagesModel);
    if (d->showDigests != showDigests) {
        qCDebug(schCore) << "Changing \"showDigests\" from" << d->showDigests << "to" << showDigests;
        d->showDigests = showDigests;
        Q_EMIT showDigestsChanged(this->showDigests());
    }
}

bool AbstractImagesModel::containsRepoTag(const QString &repo, const QString &tag) const
{
    return containsRepoTag(QLatin1String(repo.toLatin1()), QLatin1String(tag.toLatin1()));
}

bool AbstractImagesModel::containsRepoTag(QLatin1String repo, QLatin1String tag) const
{
    Q_D(const AbstractImagesModel);
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

void AbstractImagesModel::clear()
{
    Q_D(AbstractImagesModel);
    if (!d->images.empty()) {

        beginRemoveRows(QModelIndex(), 0, d->images.size() - 1);

        d->images.clear();

        endRemoveRows();
    }
}

#include "moc_abstractimagesmodel.cpp"
