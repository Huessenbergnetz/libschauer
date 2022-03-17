/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_IMAGESLISTMODEL_P_H
#define SCHAUER_IMAGESLISTMODEL_P_H

#include "imageslistmodel.h"
#include "abstractimagesmodel_p.h"

namespace Schauer {

class ImagesListModelPrivate : public AbstractImagesModelPrivate
{
public:
    ImagesListModelPrivate(ImagesListModel *q);

    ~ImagesListModelPrivate() override;

private:
    Q_DISABLE_COPY(ImagesListModelPrivate)
    Q_DECLARE_PUBLIC(ImagesListModel)
};

}

#endif // SCHAUER_IMAGESLISTMODEL_P_H
