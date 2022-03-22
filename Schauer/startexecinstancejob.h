/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_STARTEXECINSTANCEJOB_H
#define SCHAUER_STARTEXECINSTANCEJOB_H

#include "schauer_exports.h"
#include "job.h"

namespace Schauer {

class StartExecInstanceJobPrivate;

class SCHAUER_LIBRARY StartExecInstanceJob : public Job
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool detach READ detach WRITE setDetach NOTIFY detachChanged)
    Q_PROPERTY(bool tty READ tty WRITE setTty NOTIFY ttyChanged)
public:
    explicit StartExecInstanceJob(QObject *parent = nullptr);

    ~StartExecInstanceJob() override;

    void start() override;

    QString id() const;

    void setId(const QString &id);

    bool detach() const;

    void setDetach(bool detach);

    bool tty() const;

    void setTty(bool tty);

Q_SIGNALS:
    void idChanged(const QString &id);

    void detachChanged(bool detach);

    void ttyChanged(bool tty);

private:
    Q_DISABLE_COPY(StartExecInstanceJob)
    Q_DECLARE_PRIVATE_D(s_ptr, StartExecInstanceJob)
};

}

#endif // SCHAUER_STARTEXECINSTANCEJOB_H
