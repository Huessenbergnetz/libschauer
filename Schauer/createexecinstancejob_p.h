/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_CREATEEXECINSTANCEJOB_P_H
#define SCHAUER_CREATEEXECINSTANCEJOB_P_H

#include "createexecinstancejob.h"
#include "job_p.h"

namespace Schauer {

class CreateExecInstanceJobPrivate : public JobPrivate
{
public:
    CreateExecInstanceJobPrivate(CreateExecInstanceJob *q);

    ~CreateExecInstanceJobPrivate() override;

    QString buildUrlPath() const override;

    std::pair<QByteArray, QByteArray> buildPayload() const override;

    void emitDescription() override;

    bool checkInput() override;

    QString id;
    QString detachKeys;
    QString user;
    QString workingDir;
    QStringList env;
    QStringList cmd;
    bool attachStdin = false;
    bool attachStdout = false;
    bool attachStderr = false;
    bool tty = false;
    bool privileged = false;

private:
    Q_DECLARE_PUBLIC(CreateExecInstanceJob)
    Q_DISABLE_COPY(CreateExecInstanceJobPrivate)
};

}

#endif // SCHAUER_CREATEEXECINSTANCEJOB_P_H
