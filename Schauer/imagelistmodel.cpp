/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "imagelistmodel_p.h"
#include "logging.h"

using namespace Schauer;

ImageListModelPrivate::ImageListModelPrivate(ImageListModel *q)
    : AbstractImageModelPrivate(q)
{

}

ImageListModelPrivate::~ImageListModelPrivate() = default;

ImageListModel::ImageListModel(QObject *parent)
    : AbstractImageModel(* new ImageListModelPrivate(this), parent)
{

}

ImageListModel::~ImageListModel() = default;

QModelIndex ImageListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex ImageListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex ImageListModel::sibling(int row, int column, const QModelIndex &idx) const
{
    Q_UNUSED(idx);
    return index(row, column);
}

Qt::ItemFlags ImageListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        f |= Qt::ItemNeverHasChildren;
    }
    return f;
}

int ImageListModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}

bool ImageListModel::hasChildren(const QModelIndex &parent) const
{
    return parent.isValid() ? false : (rowCount() > 0);
}

QHash<int, QByteArray> ImageListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(IdRole, QByteArrayLiteral("id"));
    roles.insert(ParentIdRole, QByteArrayLiteral("parentId"));
    roles.insert(RepoTagsRole, QByteArrayLiteral("repoTags"));
    roles.insert(RepoDigestsRole, QByteArrayLiteral("repoDigests"));
    roles.insert(CreatedRole, QByteArrayLiteral("created"));
    roles.insert(SizeRole, QByteArrayLiteral("size"));
    roles.insert(VirtualSizeRole, QByteArrayLiteral("virtualSize"));
    roles.insert(SharedSizeRole, QByteArrayLiteral("sharedSize"));
    roles.insert(LabelsRole, QByteArrayLiteral("labels"));
    roles.insert(ContainersRole, QByteArrayLiteral("containers"));
    return roles;
}

QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(schCore) << "Invalid model index" << index;
        return QVariant();
    }

    if (index.row() >= rowCount()) {
        qCWarning(schCore) << "Model index" << index << "row is out of range";
        return QVariant();
    }

    Q_D(const ImageListModel);

    const ImageModelItem i = d->images.at(index.row());

    switch (role) {
    case IdRole:
        return QVariant::fromValue(i.id);
    case ParentIdRole:
        return QVariant::fromValue(i.parentId);
    case RepoTagsRole:
        return QVariant::fromValue(i.repoTags);
    case RepoDigestsRole:
        return QVariant::fromValue(i.repoDigests);
    case CreatedRole:
        return QVariant::fromValue(i.created);
    case SizeRole:
        return QVariant::fromValue(i.size);
    case VirtualSizeRole:
        return QVariant::fromValue(i.virtualSize);
    case SharedSizeRole:
        return QVariant::fromValue(i.sharedSize);
    case LabelsRole:
        return QVariant::fromValue(i.labels);
    case ContainersRole:
        return QVariant::fromValue(i.containers);
    default:
        return QVariant();
    }
}

#include "moc_imagelistmodel.cpp"
