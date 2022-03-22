/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_CREATEEXECINSTANCEJOB_H
#define SCHAUER_CREATEEXECINSTANCEJOB_H

#include "schauer_exports.h"
#include "job.h"

namespace Schauer {

class CreateExecInstanceJobPrivate;

class SCHAUER_LIBRARY CreateExecInstanceJob : public Job
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool attachStdin READ attachStdin WRITE setAttachStdin NOTIFY attachStdinChanged)
    Q_PROPERTY(bool attachStdout READ attachStdout WRITE setAttachStdout NOTIFY attachStdoutChanged)
    Q_PROPERTY(bool attachStderr READ attachStderr WRITE setAttachStderr NOTIFY attachStderrChanged)
    Q_PROPERTY(QString detachKeys READ detachKeys WRITE setDetachKeys NOTIFY detachKeysChanged)
    Q_PROPERTY(bool tty READ tty WRITE setTty NOTIFY ttyChanged)
    Q_PROPERTY(QStringList env READ env WRITE setEnv NOTIFY envChanged)
    Q_PROPERTY(QStringList cmd READ cmd WRITE setCmd NOTIFY cmdChanged)
    Q_PROPERTY(bool privileged READ privileged WRITE setPrivileged NOTIFY privilegedChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString workingDir READ workingDir WRITE setWorkingDir NOTIFY workingDirChanged)
public:
    explicit CreateExecInstanceJob(QObject *parent = nullptr);
    ~CreateExecInstanceJob() override;

    void start() override;

    QString id() const;

    void setId(const QString &id);

    bool attachStdin() const;

    void setAttachStdin(bool attachStdin);

    bool attachStdout() const;

    void setAttachStdout(bool attachStdout);

    bool attachStderr() const;

    void setAttachStderr(bool attachStderr);

    QString detachKeys() const;

    void setDetachKeys(const QString &detachKeys);

    bool tty() const;

    void setTty(bool tty);

    QStringList env() const;

    void setEnv(const QStringList &env);

    void addEnv(const QString &env);

    void removeEnv(const QString &env);

    QStringList cmd() const;

    void setCmd(const QStringList &cmd);

    void addCmd(const QString &cmd);

    void removeCmd(const QString &cmd);

    bool privileged() const;

    void setPrivileged(bool privileged);

    QString user() const;

    void setUser(const QString &user);

    QString workingDir() const;

    void setWorkingDir(const QString &workingDir);

Q_SIGNALS:
    void idChanged(const QString &id);

    void attachStdinChanged(bool attachStdin);

    void attachStdoutChanged(bool attachStdout);

    void attachStderrChanged(bool attachStderr);

    void detachKeysChanged(const QString &detachKeys);

    void ttyChanged(bool tty);

    void envChanged(const QStringList &env);

    void cmdChanged(const QStringList &cmd);

    void privilegedChanged(bool privileged);

    void userChanged(const QString &user);

    void workingDirChanged(const QString &workingDir);

private:
    Q_DISABLE_COPY(CreateExecInstanceJob)
    Q_DECLARE_PRIVATE_D(s_ptr, CreateExecInstanceJob)
};

}

#endif // SCHAUER_CREATEEXECINSTANCEJOB_H
