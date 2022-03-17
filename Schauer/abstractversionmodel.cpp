/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractversionmodel_p.h"
#include "getversionjob.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

using namespace Schauer;

AbstractVersionModelPrivate::AbstractVersionModelPrivate(AbstractVersionModel *q)
    : AbstractBaseModelPrivate(q)
{

}

AbstractVersionModelPrivate::~AbstractVersionModelPrivate() = default;

void AbstractVersionModelPrivate::setupJob()
{
    Q_Q(AbstractVersionModel);
    job = new GetVersionJob(q);
}

bool AbstractVersionModelPrivate::loadFromJson(const QJsonDocument &json)
{
    Q_Q(AbstractVersionModel);

    const QJsonObject versionObject = json.object();
    apiVersion = versionObject.value(QStringLiteral("ApiVersion")).toString();
    arch = versionObject.value(QStringLiteral("Arch")).toString();
    buildTime = QDateTime::fromString(versionObject.value(QStringLiteral("BuildTime")).toString(), Qt::ISODateWithMs);
    gitCommit = versionObject.value(QStringLiteral("GitCommit")).toString();
    goVersion = versionObject.value(QStringLiteral("GoVersion")).toString();
    kernelVersion = versionObject.value(QStringLiteral("KernelVersion")).toString();
    minApiVersion = versionObject.value(QStringLiteral("MinAPIVersion")).toString();
    os = versionObject.value(QStringLiteral("Os")).toString();
    platformName = versionObject.value(QStringLiteral("Platform")).toObject().value(QStringLiteral("Name")).toString();
    version = versionObject.value(QStringLiteral("Version")).toString();

    const QJsonArray comps = versionObject.value(QStringLiteral("Components")).toArray();

    q->beginInsertRows(QModelIndex(), components.size(), components.size() + comps.size() - 1);

    for (const QJsonValue &v : comps) {
        const QJsonObject o = v.toObject();
        const QString name = o.value(QStringLiteral("Name")).toString();
        const QString version = o.value(QStringLiteral("Version")).toString();
        const QJsonObject d = o.value(QStringLiteral("Details")).toObject();
        const QString apiVersion = d.value(QStringLiteral("ApiVersion")).toString();
        const QString arch = d.value(QStringLiteral("Arch")).toString();
        const QDateTime buildTime = d.contains(QStringLiteral("BuildTime")) ? QDateTime::fromString(d.value(QStringLiteral("BuildTime")).toString(), Qt::ISODateWithMs) : QDateTime();
        const bool experimental = d.value(QStringLiteral("Experimental")).toBool();
        const QString gitCommit = d.value(QStringLiteral("GitCommit")).toString();
        const QString goVersion = d.value(QStringLiteral("GoVersion")).toString();
        const QString kernelVersion = d.value(QStringLiteral("KernelVersion")).toString();
        const QString minApiVersion = d.value(QStringLiteral("MinAPIVersion")).toString();
        const QString os = d.value(QStringLiteral("Os")).toString();

        components.emplace_back(name, apiVersion, arch, buildTime, gitCommit, goVersion, kernelVersion, minApiVersion, os, version, experimental);
    }

    q->endInsertRows();

    Q_EMIT q->loaded();

    setIsLoading(false);

    return true;
}

AbstractVersionModel::AbstractVersionModel(QObject *parent)
    : AbstractBaseModel(* new AbstractVersionModelPrivate(this), parent)
{

}

AbstractVersionModel::AbstractVersionModel(AbstractVersionModelPrivate &dd, QObject *parent)
    : AbstractBaseModel(dd, parent)
{

}

AbstractVersionModel::~AbstractVersionModel() = default;

QString AbstractVersionModel::apiVersion() const
{
    Q_D(const AbstractVersionModel);
    return d->apiVersion;
}

QString AbstractVersionModel::arch() const
{
    Q_D(const AbstractVersionModel);
    return d->arch;
}

QDateTime AbstractVersionModel::buildTime() const
{
    Q_D(const AbstractVersionModel);
    return d->buildTime;
}

QString AbstractVersionModel::gitCommit() const
{
    Q_D(const AbstractVersionModel);
    return d->gitCommit;
}

QString AbstractVersionModel::goVersion() const
{
    Q_D(const AbstractVersionModel);
    return d->goVersion;
}

QString AbstractVersionModel::kernelVersion() const
{
    Q_D(const AbstractVersionModel);
    return d->kernelVersion;
}

QString AbstractVersionModel::minApiVersion() const
{
    Q_D(const AbstractVersionModel);
    return d->minApiVersion;
}

QString AbstractVersionModel::os() const
{
    Q_D(const AbstractVersionModel);
    return d->os;
}

QString AbstractVersionModel::platformName() const
{
    Q_D(const AbstractVersionModel);
    return d->platformName;
}

QString AbstractVersionModel::version() const
{
    Q_D(const AbstractVersionModel);
    return d->version;
}

int AbstractVersionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_D(const AbstractVersionModel);
    return d->components.size();
}

void AbstractVersionModel::clear()
{
    Q_D(AbstractVersionModel);
    if (!d->components.empty()) {
        beginRemoveRows(QModelIndex(), 0, d->components.size() - 1);

        d->components.clear();

        endRemoveRows();
    }
}

#include "moc_abstractversionmodel.cpp"
