/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "containerlistmodel_p.h"
#include "logging.h"

using namespace Schauer;

ContainerListModelPrivate::ContainerListModelPrivate(ContainerListModel *q)
    : AbstractContainerModelPrivate(q)
{

}

ContainerListModelPrivate::~ContainerListModelPrivate() = default;

ContainerListModel::ContainerListModel(QObject *parent)
    : AbstractContainerModel(* new ContainerListModelPrivate(this), parent)
{

}

ContainerListModel::~ContainerListModel() = default;

QModelIndex ContainerListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex ContainerListModel::parent([[maybe_unused]] const QModelIndex &child) const
{
    return QModelIndex();
}

QModelIndex ContainerListModel::sibling(int row, int column, [[maybe_unused]] const QModelIndex &idx) const
{
    return index(row, column);
}

Qt::ItemFlags ContainerListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        f |= Qt::ItemNeverHasChildren;
    }
    return f;
}

int ContainerListModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}

bool ContainerListModel::hasChildren(const QModelIndex &parent) const
{
    return parent.isValid() ? false : (rowCount() > 0);
}

QHash<int, QByteArray> ContainerListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(IdRole, QByteArrayLiteral("id"));
    roles.insert(NamesRole, QByteArrayLiteral("names"));
    roles.insert(ImageRole, QByteArrayLiteral("image"));
    roles.insert(ImageIdRole, QByteArrayLiteral("imageId"));
    roles.insert(CommandRole, QByteArrayLiteral("command"));
    roles.insert(CreatedRole, QByteArrayLiteral("created"));
    roles.insert(StateRole, QByteArrayLiteral("state"));
    roles.insert(StatusRole, QByteArrayLiteral("status"));
    roles.insert(LabelsRole, QByteArrayLiteral("labels"));
    roles.insert(SizeRwRole, QByteArrayLiteral("sizeRw"));
    roles.insert(SizeRootFsRole, QByteArrayLiteral("sizeRootFs"));
    return roles;
}

QVariant ContainerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(schCore) << "Invalid model index" << index;
        return QVariant();
    }

    if (index.row() >= rowCount()) {
        qCWarning(schCore) << "Model index" << index << "row is out or range";
        return QVariant();
    }

    Q_D(const ContainerListModel);

    const ContainerModelItem c = d->containers.at(index.row());

    switch (role) {
    case IdRole:
        return QVariant::fromValue(c.id);
    case NamesRole:
        return QVariant::fromValue(c.names);
    case ImageRole:
        return QVariant::fromValue(c.image);
    case ImageIdRole:
        return QVariant::fromValue(c.imageId);
    case CommandRole:
        return QVariant::fromValue(c.command);
    case CreatedRole:
        return QVariant::fromValue(c.created);
    case StateRole:
        return QVariant::fromValue(c.state);
    case StatusRole:
        return QVariant::fromValue(c.status);
    case LabelsRole:
        return QVariant::fromValue(c.labels);
    case SizeRwRole:
        return QVariant::fromValue(c.sizeRw);
    case SizeRootFsRole:
        return QVariant::fromValue(c.sizeRootFs);
    default:
        return QVariant();
    }
}

#include "moc_containerlistmodel.cpp"
