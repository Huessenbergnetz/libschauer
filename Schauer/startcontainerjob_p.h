/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_STARTCONTAINERJOB_P_H
#define SCHAUER_STARTCONTAINERJOB_P_H

#include "startcontainerjob.h"
#include "job_p.h"

namespace Schauer {

class StartContainerJobPrivate : public JobPrivate
{
public:
    explicit StartContainerJobPrivate(StartContainerJob *q);

    ~StartContainerJobPrivate() override;

    QString buildUrlPath() const override;

    QUrlQuery buildUrlQuery() const override;

    void emitDescription() override;

    bool checkInput() override;

    QString id;
    QString detachKeys;

private:
    Q_DECLARE_PUBLIC(StartContainerJob)
    Q_DISABLE_COPY(StartContainerJobPrivate)
};

}

#endif // SCHAUER_STARTCONTAINERJOB_P_H
