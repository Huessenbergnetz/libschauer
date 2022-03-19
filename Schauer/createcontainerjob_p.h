/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_CREATECONTAINERJOB_P_H
#define SCHAUER_CREATECONTAINERJOB_P_H

#include "createcontainerjob.h"
#include "job_p.h"

namespace Schauer {

class CreateContainerJobPrivate : public JobPrivate
{
public:
    explicit CreateContainerJobPrivate(CreateContainerJob *q);

    ~CreateContainerJobPrivate() override;

    QString buildUrlPath() const override;

    QUrlQuery buildUrlQuery() const override;

    std::pair<QByteArray, QByteArray> buildPayload() const override;

    void emitDescription() override;

    bool checkInput() override;

    QString name;
    QVariantHash containerConfig;

private:
    Q_DISABLE_COPY(CreateContainerJobPrivate)
    Q_DECLARE_PUBLIC(CreateContainerJob)
};

}

#endif // SCHAUER_CREATECONTAINERJOB_P_H
