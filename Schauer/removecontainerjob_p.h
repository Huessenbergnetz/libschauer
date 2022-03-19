/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_REMOVECONTAINERJOB_P_H
#define SCHAUER_REMOVECONTAINERJOB_P_H

#include "removecontainerjob.h"
#include "job_p.h"

namespace Schauer {

class RemoveContainerJobPrivate : public JobPrivate
{
public:
    explicit RemoveContainerJobPrivate(RemoveContainerJob *q);

    ~RemoveContainerJobPrivate() override;

    QString buildUrlPath() const override;

    QUrlQuery buildUrlQuery() const override;

    void emitDescription() override;

    bool checkInput() override;

    QString id;
    bool removeAnonVolumes = false;
    bool force = false;
    bool removeLinks = false;

private:
    Q_DECLARE_PUBLIC(RemoveContainerJob)
    Q_DISABLE_COPY(RemoveContainerJobPrivate)
};

}

#endif // SCHAUER_REMOVECONTAINERJOB_P_H
