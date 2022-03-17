/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "versionlistmodel_p.h"

using namespace Schauer;

VersionListModelPrivate::VersionListModelPrivate(VersionListModel *q)
    : AbstractVersionModelPrivate(q)
{

}

VersionListModelPrivate::~VersionListModelPrivate() = default;

VersionListModel::VersionListModel(QObject *parent) : AbstractVersionModel(parent)
{

}

VersionListModel::~VersionListModel() = default;

QModelIndex VersionListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex VersionListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex VersionListModel::sibling(int row, int column, const QModelIndex &idx) const
{
    Q_UNUSED(idx);
    return index(row, column);
}

Qt::ItemFlags VersionListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        f |= Qt::ItemNeverHasChildren;
    }

    return f;
}

int VersionListModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}

bool VersionListModel::hasChildren(const QModelIndex &parent) const
{
    return parent.isValid() ? false : (rowCount() > 0);
}

QHash<int, QByteArray> VersionListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(NameRole, QByteArrayLiteral("name"));
    roles.insert(VersionRole, QByteArrayLiteral("version"));
    roles.insert(ApiVersionRole, QByteArrayLiteral("apiVersion"));
    roles.insert(ArchRole, QByteArrayLiteral("arch"));
    roles.insert(BuildTimeRole, QByteArrayLiteral("buildTime"));
    roles.insert(ExperimentalRole, QByteArrayLiteral("experimental"));
    roles.insert(GitCommitRole, QByteArrayLiteral("gitCommit"));
    roles.insert(GoVersionRole, QByteArrayLiteral("goVersion"));
    roles.insert(KernelVersionRole, QByteArrayLiteral("kernelVersion"));
    roles.insert(MinAPIVersionRole, QByteArrayLiteral("minApiVersion"));
    roles.insert(OsRole, QByteArrayLiteral("os"));
    return roles;
}

QVariant VersionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount()) {
        return QVariant();
    }

    Q_D(const VersionListModel);

    const VersionComponent c = d->components.at(index.row());

    switch(role) {
    case NameRole:
        return QVariant::fromValue(c.name);
    case VersionRole:
        return QVariant::fromValue(c.version);
    case ApiVersionRole:
        return QVariant::fromValue(c.apiVersion);
    case ArchRole:
        return QVariant::fromValue(c.arch);
    case BuildTimeRole:
        return QVariant::fromValue(c.buildTime);
    case ExperimentalRole:
        return QVariant::fromValue(c.experimental);
    case GitCommitRole:
        return QVariant::fromValue(c.gitCommit);
    case GoVersionRole:
        return QVariant::fromValue(c.goVersion);
    case KernelVersionRole:
        return QVariant::fromValue(c.kernelVersion);
    case MinAPIVersionRole:
        return QVariant::fromValue(c.minApiVersion);
    case OsRole:
        return QVariant::fromValue(c.os);
    default:
        return QVariant();
    }
}

#include "moc_versionlistmodel.cpp"
