/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_STOPCONTAINERJOB_P_H
#define SCHAUER_STOPCONTAINERJOB_P_H

#include "stopcontainerjob.h"
#include "job_p.h"

namespace Schauer {

class StopContainerJobPrivate : public JobPrivate
{
public:
    explicit StopContainerJobPrivate(StopContainerJob *q);

    ~StopContainerJobPrivate() override;

    QString buildUrlPath() const override;

    QUrlQuery buildUrlQuery() const override;

    void emitDescription() override;

    bool checkInput() override;

    QString id;
    int timeout = 0;

private:
    Q_DECLARE_PUBLIC(StopContainerJob)
    Q_DISABLE_COPY(StopContainerJobPrivate)
};

}

#endif // SCHAUER_STOPCONTAINERJOB_P_H
