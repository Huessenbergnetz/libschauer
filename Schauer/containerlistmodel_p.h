/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_CONTAINERLISTMODEL_P_H
#define SCHAUER_CONTAINERLISTMODEL_P_H

#include "containerlistmodel.h"
#include "abstractcontainermodel_p.h"

namespace Schauer {

class ContainerListModelPrivate : public AbstractContainerModelPrivate
{
public:
    ContainerListModelPrivate(ContainerListModel *q);

    ~ContainerListModelPrivate() override;

private:
    Q_DISABLE_COPY(ContainerListModelPrivate)
    Q_DECLARE_PUBLIC(ContainerListModel)
};

}

#endif // SCHAUER_CONTAINERLISTMODEL_P_H
