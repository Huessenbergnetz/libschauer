/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTIMAGEMODEL_P_H
#define SCHAUER_ABSTRACTIMAGEMODEL_P_H

#include "abstractimagemodel.h"
#include "abstractbasemodel_p.h"
#include <QDateTime>
#include <QMap>
#include <vector>

namespace Schauer {

struct ImageModelItem {
    QString id;
    QString parentId;
    QStringList repoTags;
    QStringList repoDigests;
    QDateTime created;
    qint64 size;
    qint64 virtualSize;
    qint64 sharedSize;
    QMap<QString,QString> labels;
    int containers = 0;

    ImageModelItem(const QString _id, const QString &_parentId, const QStringList &_repoTags, const QStringList &_repoDigests, const QDateTime &_created, qint64 _size, qint64 _virtualSize, qint64 _sharedSize, const QMap<QString,QString> &_labels, int _containers) :
        id(_id),
        parentId(_parentId),
        repoTags(_repoTags),
        repoDigests(_repoDigests),
        created(_created),
        size(_size),
        virtualSize(_virtualSize),
        sharedSize(_sharedSize),
        labels(_labels),
        containers(_containers)
    {}

    ~ImageModelItem() = default;

    ImageModelItem(const ImageModelItem &other) :
        id(other.id),
        parentId(other.parentId),
        repoTags(other.repoTags),
        repoDigests(other.repoDigests),
        created(other.created),
        size(other.size),
        virtualSize(other.virtualSize),
        sharedSize(other.sharedSize),
        labels(other.labels),
        containers(other.containers)
    {}

    ImageModelItem(ImageModelItem &&other) :
        id(std::move(other.id)),
        parentId(std::move(other.parentId)),
        repoTags(std::move(other.repoTags)),
        repoDigests(std::move(other.repoDigests)),
        created(std::move(other.created)),
        size(std::move(other.size)),
        virtualSize(std::move(other.virtualSize)),
        sharedSize(std::move(other.sharedSize)),
        labels(std::move(other.labels)),
        containers(std::move(other.containers))
    {}

    ImageModelItem& operator=(const ImageModelItem &other) = default;

    ImageModelItem& operator=(ImageModelItem &&other) = default;
};

class AbstractImageModelPrivate : public AbstractBaseModelPrivate
{
public:
    AbstractImageModelPrivate(AbstractImageModel *q);

    ~AbstractImageModelPrivate();

    void setupJob() override;
    bool loadFromJson(const QJsonDocument &json) override;

    std::vector<ImageModelItem> images;
    bool showAll = false;
    bool showDigests = false;

private:
    Q_DISABLE_COPY(AbstractImageModelPrivate)
    Q_DECLARE_PUBLIC(AbstractImageModel)
};

}

#endif // SCHAUER_ABSTRACTIMAGEMODEL_P_H
