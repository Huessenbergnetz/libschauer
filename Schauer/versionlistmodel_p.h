/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_VERSIONLISTMODEL_P_H
#define SCHAUER_VERSIONLISTMODEL_P_H

#include "versionlistmodel.h"
#include "abstractversionmodel_p.h"

namespace Schauer {

class VersionListModelPrivate : public AbstractVersionModelPrivate
{
public:
    VersionListModelPrivate(VersionListModel *q);

    ~VersionListModelPrivate();

private:
    Q_DISABLE_COPY(VersionListModelPrivate)
    Q_DECLARE_PUBLIC(VersionListModel)
};

}

#endif // SCHAUER_VERSIONLISTMODEL_P_H
