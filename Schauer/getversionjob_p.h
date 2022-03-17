/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_GETVERSIONJOB_P_H
#define SCHAUER_GETVERSIONJOB_P_H

#include "getversionjob.h"
#include "job_p.h"

namespace Schauer {

class GetVersionJobPrivate : public JobPrivate
{
public:
    explicit GetVersionJobPrivate(GetVersionJob *q);
    ~GetVersionJobPrivate() override;

    QString buildUrlPath() const override;

    void emitDescription() override;

private:
    Q_DISABLE_COPY(GetVersionJobPrivate)
    Q_DECLARE_PUBLIC(GetVersionJob)
};

}

#endif // SCHAUER_GETVERSIONJOB_P_H
