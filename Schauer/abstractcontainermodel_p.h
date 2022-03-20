/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTCONTAINERMODEL_P_H
#define SCHAUER_ABSTRACTCONTAINERMODEL_P_H

#include "abstractcontainermodel.h"
#include "abstractbasemodel_p.h"
#include <QDateTime>

namespace Schauer {

struct ContainerModelItem {
    QString id;
    QStringList names;
    QString image;
    QString imageId;
    QString command;
    QDateTime created;
    QString state;
    QString status;
    QMap<QString,QString> labels;
    quint64 sizeRw = 0;
    quint64 sizeRootFs = 0;

    ContainerModelItem(const QString &_id, const QStringList &_names, const QString &_image, const QString &_imageId, const QString &_command, const QDateTime &_created, const QString &_state, const QString &_status, const QMap<QString,QString> &_labels, quint64 _sizeRw, quint64 _sizeRootFs) :
        id{_id},
        names{_names},
        image{_image},
        imageId{_imageId},
        command{_command},
        created{_created},
        state{_state},
        status{_status},
        labels{_labels},
        sizeRw{_sizeRw},
        sizeRootFs{_sizeRootFs}
    {}

    ~ContainerModelItem() = default;

    ContainerModelItem(const ContainerModelItem &other) :
        id{other.id},
        names{other.names},
        image{other.image},
        imageId{other.imageId},
        command{other.command},
        created{other.created},
        state{other.state},
        status{other.status},
        labels{other.labels},
        sizeRw{other.sizeRw},
        sizeRootFs{other.sizeRootFs}
    {}

    ContainerModelItem(ContainerModelItem &&other) :
        id(std::move(other.id)),
        names(std::move(other.names)),
        image(std::move(other.image)),
        imageId(std::move(other.imageId)),
        command(std::move(other.command)),
        created(std::move(other.created)),
        state(std::move(other.state)),
        status(std::move(other.status)),
        labels(std::move(other.labels)),
        sizeRw(std::move(other.sizeRw)),
        sizeRootFs(std::move(other.sizeRootFs))
    {}

    ContainerModelItem& operator=(const ContainerModelItem &other) = default;

    ContainerModelItem& operator=(ContainerModelItem &&other) = default;
};

class AbstractContainerModelPrivate : public AbstractBaseModelPrivate
{
public:
    AbstractContainerModelPrivate(AbstractContainerModel *q);

    ~AbstractContainerModelPrivate();

    void setupJob() override;

    bool loadFromJson(const QJsonDocument &json) override;

    bool contains(QLatin1String idOrName) const;

    bool containsImage(QLatin1String image) const;

    bool containsImageId(QLatin1String imageId) const;

    std::vector<ContainerModelItem> containers;
    bool showAll = false;
    bool showSize = false;

private:
    Q_DISABLE_COPY(AbstractContainerModelPrivate)
    Q_DECLARE_PUBLIC(AbstractContainerModel)
};

}

#endif // SCHAUER_ABSTRACTCONTAINERMODEL_P_H
