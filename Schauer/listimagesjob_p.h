/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_LISTIMAGESJOB_P_H
#define SCHAUER_LISTIMAGESJOB_P_H

#include "listimagesjob.h"
#include "job_p.h"

namespace Schauer {

class ListImagesJobPrivate : public JobPrivate
{
public:
    explicit ListImagesJobPrivate(ListImagesJob *q);

    ~ListImagesJobPrivate() override;

    QString buildUrlPath() const override;

    QUrlQuery buildUrlQuery() const override;

    void emitDescription() override;

    bool showAll = false;
    bool showDigests = false;

private:
    Q_DISABLE_COPY(ListImagesJobPrivate)
    Q_DECLARE_PUBLIC(ListImagesJob)
};

}

#endif // SCHAUER_LISTIMAGESJOB_P_H
