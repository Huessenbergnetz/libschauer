/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractcontainermodel_p.h"
#include "listcontainersjob.h"
#include "logging.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

using namespace Schauer;

AbstractContainerModelPrivate::AbstractContainerModelPrivate(AbstractContainerModel *q)
    : AbstractBaseModelPrivate(q)
{

}

AbstractContainerModelPrivate::~AbstractContainerModelPrivate() = default;

void AbstractContainerModelPrivate::setupJob()
{
    Q_Q(AbstractContainerModel);
    auto _job = new ListContainersJob(q);
    _job->setShowAll(showAll);
    _job->setShowSize(showSize);
    job = _job;
}

bool AbstractContainerModelPrivate::loadFromJson(const QJsonDocument &json)
{
    Q_Q(AbstractContainerModel);

    const QJsonArray conts = json.array();

    q->beginInsertRows(QModelIndex(), containers.size(), containers.size() + conts.size() - 1);

    for (const QJsonValue &cont : conts) {
        const QJsonObject o = cont.toObject();

        const QString id = o.value(QStringLiteral("Id")).toString();
        const QStringList names = AbstractBaseModelPrivate::jsonArrayToStringList(o.value(QStringLiteral("Names")));
        const QString image = o.value(QStringLiteral("Image")).toString();
        const QString imageId = o.value(QStringLiteral("ImageID")).toString();
        const QString command = o.value(QStringLiteral("Command")).toString();
        const QDateTime created = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(o.value(QStringLiteral("Created")).toDouble()), Qt::UTC);
        const QString state = o.value(QStringLiteral("State")).toString();
        const QString status = o.value(QStringLiteral("Status")).toString();
        const QMap<QString,QString> labels = AbstractBaseModelPrivate::jsonObjectToStringMap(o.value(QStringLiteral("Labels")));
        const quint64 sizeRw = static_cast<quint64>(o.value(QStringLiteral("SizeRw")).toDouble());
        const quint64 sizeRootFs = static_cast<quint64>(o.value(QStringLiteral("SizeRootFs")).toDouble());

        containers.emplace_back(id, names, image, imageId, command, created, state, status, labels, sizeRw, sizeRootFs);
    }

    q->endInsertRows();

    Q_EMIT q->loaded();

    setIsLoading(false);

    return true;
}

bool AbstractContainerModelPrivate::contains(QLatin1String idOrName) const
{
    if (containers.empty() || idOrName.isEmpty()) {
        return false;
    }

    if (idOrName.startsWith(QLatin1Char('/'))) {
        for (const ContainerModelItem &item : static_cast<const std::vector<ContainerModelItem>&>(containers)) {
            for (const QString &name : static_cast<const QStringList &>(item.names)) {
                if (name == idOrName) {
                    return true;
                }
            }
        }
    } else {
        for (const ContainerModelItem &item : static_cast<const std::vector<ContainerModelItem>&>(containers)) {
            if (item.id == idOrName) {
                return true;
            }
        }
    }

    return false;
}

bool AbstractContainerModelPrivate::containsImage(QLatin1String image) const
{
    if (containers.empty() || image.isEmpty()) {
        return false;
    }

    for (const ContainerModelItem &item : static_cast<const std::vector<ContainerModelItem>&>(containers)) {
        if (item.image == image) {
            return true;
        }
    }

    return false;
}

bool AbstractContainerModelPrivate::containsImageId(QLatin1String imageId) const
{
    if (containers.empty() || imageId.isEmpty()) {
        return false;
    }

    for (const ContainerModelItem &item : static_cast<const std::vector<ContainerModelItem>&>(containers)) {
        if (item.imageId == imageId) {
            return true;
        }
    }

    return false;
}

AbstractContainerModel::AbstractContainerModel(QObject *parent)
    : AbstractBaseModel(* new AbstractContainerModelPrivate(this), parent)
{

}

AbstractContainerModel::AbstractContainerModel(AbstractContainerModelPrivate &dd, QObject *parent)
    : AbstractBaseModel(dd, parent)
{

}

AbstractContainerModel::~AbstractContainerModel() = default;

int AbstractContainerModel::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    Q_D(const AbstractContainerModel);
    return d->containers.size();
}

bool AbstractContainerModel::showAll() const
{
    Q_D(const AbstractContainerModel);
    return d->showAll;
}

void AbstractContainerModel::setShowAll(bool showAll)
{
    Q_D(AbstractContainerModel);
    if (d->showAll != showAll) {
        qCDebug(schCore) << "Changing \"showAll\" from" << d->showAll << "to" << showAll;
        d->showAll = showAll;
        Q_EMIT showAllChanged(this->showAll());
    }
}

bool AbstractContainerModel::showSize() const
{
    Q_D(const AbstractContainerModel);
    return d->showSize;
}

void AbstractContainerModel::setShowSize(bool showSize)
{
    Q_D(AbstractContainerModel);
    if (d->showSize != showSize) {
        qCDebug(schCore) << "Changing \"showSize\" from" << d->showSize << "to" << showSize;
        d->showSize = showSize;
        Q_EMIT showSizeChanged(this->showSize());
    }
}

bool AbstractContainerModel::contains(const QString &idOrName) const
{
    Q_D(const AbstractContainerModel);
    return d->contains(QLatin1String(idOrName.toLatin1()));
}

bool AbstractContainerModel::contains(QLatin1String idOrName) const
{
    Q_D(const AbstractContainerModel);
    return d->contains(idOrName);
}

bool AbstractContainerModel::containsImage(const QString &image) const
{
    Q_D(const AbstractContainerModel);
    return d->containsImage(QLatin1String(image.toLatin1()));
}

bool AbstractContainerModel::containsImage(QLatin1String &image) const
{
    Q_D(const AbstractContainerModel);
    return d->containsImage(image);
}

bool AbstractContainerModel::containsImageId(const QString &imageId) const
{
    Q_D(const AbstractContainerModel);
    return d->containsImageId(QLatin1String(imageId.toLatin1()));
}

bool AbstractContainerModel::containsImageId(QLatin1String &imageId) const
{
    Q_D(const AbstractContainerModel);
    return d->containsImageId(imageId);
}

void AbstractContainerModel::clear()
{
    Q_D(AbstractContainerModel);
    if (!d->containers.empty()) {
        beginRemoveRows(QModelIndex(), 0, d->containers.size() - 1);

        d->containers.clear();

        endRemoveRows();
    }
}

#include "moc_abstractcontainermodel.cpp"
