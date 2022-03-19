/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_IMAGESLISTMODEL_P_H
#define SCHAUER_IMAGESLISTMODEL_P_H

#include "imagelistmodel.h"
#include "abstractimagesmodel_p.h"

namespace Schauer {

class ImageListModelPrivate : public AbstractImagesModelPrivate
{
public:
    ImageListModelPrivate(ImageListModel *q);

    ~ImageListModelPrivate() override;

private:
    Q_DISABLE_COPY(ImageListModelPrivate)
    Q_DECLARE_PUBLIC(ImageListModel)
};

}

#endif // SCHAUER_IMAGESLISTMODEL_P_H
