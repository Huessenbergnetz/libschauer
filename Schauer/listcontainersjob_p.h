/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_LISTCONTAINERSJOB_P_H
#define SCHAUER_LISTCONTAINERSJOB_P_H

#include "listcontainersjob.h"
#include "job_p.h"

namespace Schauer {

class ListContainersJobPrivate : public JobPrivate
{
public:
    explicit ListContainersJobPrivate(ListContainersJob *q);

    ~ListContainersJobPrivate() override;

    QString buildUrlPath() const override;

    QUrlQuery buildUrlQuery() const override;

    void emitDescription() override;

    int limit = 0;
    bool showAll = false;
    bool showSize = false;

private:
    Q_DISABLE_COPY(ListContainersJobPrivate)
    Q_DECLARE_PUBLIC(ListContainersJob)
};

}

#endif // SCHAUER_LISTCONTAINERSJOB_P_H
