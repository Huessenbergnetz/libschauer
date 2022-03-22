/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_STARTEXECINSTANCEJOB_P_H
#define SCHAUER_STARTEXECINSTANCEJOB_P_H

#include "startexecinstancejob.h"
#include "job_p.h"

namespace Schauer {

class StartExecInstanceJobPrivate : public JobPrivate
{
public:
    explicit StartExecInstanceJobPrivate(StartExecInstanceJob *q);

    ~StartExecInstanceJobPrivate() override;

    QString buildUrlPath() const override;

    std::pair<QByteArray, QByteArray> buildPayload() const override;

    void emitDescription() override;

    bool checkInput() override;

    QString id;
    bool detach = false;
    bool tty = false;

private:
    Q_DISABLE_COPY(StartExecInstanceJobPrivate)
    Q_DECLARE_PUBLIC(StartExecInstanceJob)
};

}

#endif // SCHAUER_STARTEXECINSTANCEJOB_P_H
