/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "createexecinstancejob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Schauer;

CreateExecInstanceJobPrivate::CreateExecInstanceJobPrivate(CreateExecInstanceJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Post;
    expectedContentType = ExpectedContentType::JsonObject;
    requiresAuth = false;
}

CreateExecInstanceJobPrivate::~CreateExecInstanceJobPrivate() = default;

QString CreateExecInstanceJobPrivate::buildUrlPath() const
{
    const QString _id = id.startsWith(QLatin1Char('/')) ? id.mid(1) : id;
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/containers/") + _id + QLatin1String("/exec");
    return path;
}

std::pair<QByteArray,QByteArray> CreateExecInstanceJobPrivate::buildPayload() const
{
    const QJsonObject body({
                               {QStringLiteral("AttachStdin"), attachStdin},
                               {QStringLiteral("AttachStdout"), attachStdout},
                               {QStringLiteral("AttachStderr"), attachStderr},
                               {QStringLiteral("DetachKeys"), detachKeys},
                               {QStringLiteral("Tty"), tty},
                               {QStringLiteral("Env"), QJsonArray::fromStringList(env)},
                               {QStringLiteral("Cmd"), QJsonArray::fromStringList(cmd)},
                               {QStringLiteral("Privileged"), privileged},
                               {QStringLiteral("User"), user},
                               {QStringLiteral("WorkingDir"), workingDir}
                           });
    const QJsonDocument bodyDoc(body);

    return std::make_pair(bodyDoc.toJson(QJsonDocument::Compact), QByteArrayLiteral("application/json"));
}

void CreateExecInstanceJobPrivate::emitDescription()
{
    Q_Q(CreateExecInstanceJob);

    //: Job description title, %1 will be replaced by container id/name/digest
    //% "Creating new execution instance for container %1"
    const QString _title = qtTrId("libschauer-job-desc-create-exec-instance").arg(id);

    Q_EMIT q->description(q, _title);
}

bool CreateExecInstanceJobPrivate::checkInput()
{
    if (!JobPrivate::checkInput()) {
        return false;
    }

    if (id.isEmpty()) {
        //: Error message when creating a new exec instance
        //% "Can not create a new execution instance without a valid container ID."
        emitError(InvalidInput, qtTrId("libschauer-create-exec-instance-err-empty-id"));
        qCCritical(schCore) << "Missing container ID when trying to create new execution instance.";
        return false;
    }

    if (!detachKeys.isEmpty()) {
        const QString reStr = QStringLiteral("^[a-zA-Z]$|^ctrl-[a-z@\\^\\[,_]$");
        static QRegularExpression re(reStr);
        if (!detachKeys.contains(re)) {
            //: Error message if exec instance detachKeys property is malformed
            //% "Invalid “detachKeys” parameter. Format is a single charachter a-Z or “ctrl-<value>” where <value> is one of: a-z, @, ^, [, _ or ,."
            emitError(InvalidInput, qtTrId("libschauer-create-exec-instance-err-invalid-detachkeys"));
            qCCritical(schCore) << "Invalid detachKeys format when trying to create exec instance. Format is a single charachter a-Z or “ctrl-<value>” where <value> is one of: a-z, @, ^, [, _ or ,.";
            return false;
        }
    }

    if (cmd.empty()) {
        //% "Can not create a new execution instance without any command to execute."
        emitError(InvalidInput, qtTrId("libschauer-create-exec-instance-err-empty-cmd"));
        qCCritical(schCore) << "Missing command to execute when trying to create a new exec instance.";
        return false;
    }

    return true;
}

CreateExecInstanceJob::CreateExecInstanceJob(QObject *parent)
    : Job(* new CreateExecInstanceJobPrivate(this), parent)
{

}

CreateExecInstanceJob::~CreateExecInstanceJob() = default;

void CreateExecInstanceJob::start()
{
    QTimer::singleShot(0, this, &CreateExecInstanceJob::sendRequest);
}

QString CreateExecInstanceJob::id() const
{
    Q_D(const CreateExecInstanceJob);
    return d->id;
}

void CreateExecInstanceJob::setId(const QString &id)
{
    Q_D(CreateExecInstanceJob);
    if (d->id != id) {
        qCDebug(schCore) << "Changing \"id\" from" << d->id << "to" << id;
        d->id = id;
        Q_EMIT idChanged(this->id());
    }
}

bool CreateExecInstanceJob::attachStdin() const
{
    Q_D(const CreateExecInstanceJob);
    return d->attachStdin;
}

void CreateExecInstanceJob::setAttachStdin(bool attachStdin)
{
    Q_D(CreateExecInstanceJob);
    if (d->attachStdin != attachStdin) {
        qCDebug(schCore) << "Changing \"attachStdin\" from" << d->attachStdin << "to" << attachStdin;
        d->attachStdin = attachStdin;
        Q_EMIT attachStdinChanged(this->attachStdin());
    }
}

bool CreateExecInstanceJob::attachStdout() const
{
    Q_D(const CreateExecInstanceJob);
    return d->attachStdout;
}

void CreateExecInstanceJob::setAttachStdout(bool attachStdout)
{
    Q_D(CreateExecInstanceJob);
    if (d->attachStdout != attachStdout) {
        qCDebug(schCore) << "Changing \"attachStdout\" from" << d->attachStdout << "to" << attachStdout;
        d->attachStdout = attachStdout;
        Q_EMIT attachStdoutChanged(this->attachStdout());
    }
}

bool CreateExecInstanceJob::attachStderr() const
{
    Q_D(const CreateExecInstanceJob);
    return d->attachStderr;
}

void CreateExecInstanceJob::setAttachStderr(bool attachStderr)
{
    Q_D(CreateExecInstanceJob);
    if (d->attachStderr != attachStderr) {
        qCDebug(schCore) << "Changing \"attachStderr\" from" << d->attachStderr << "to" << attachStderr;
        d->attachStderr = attachStderr;
        Q_EMIT attachStderrChanged(this->attachStderr());
    }
}

QString CreateExecInstanceJob::detachKeys() const
{
    Q_D(const CreateExecInstanceJob);
    return d->detachKeys;
}

void CreateExecInstanceJob::setDetachKeys(const QString &detachKeys)
{
    Q_D(CreateExecInstanceJob);
    if (d->detachKeys != detachKeys) {
        qCDebug(schCore) << "Changing \"detachKeys\" from" << d->detachKeys << "to" << detachKeys;
        d->detachKeys = detachKeys;
        Q_EMIT detachKeysChanged(this->detachKeys());
    }
}

bool CreateExecInstanceJob::tty() const
{
    Q_D(const CreateExecInstanceJob);
    return d->tty;
}

void CreateExecInstanceJob::setTty(bool tty)
{
    Q_D(CreateExecInstanceJob);
    if (d->tty != tty) {
        qCDebug(schCore) << "Changing \"tty\" from " << d->tty << "to" << tty;
        d->tty = tty;
        Q_EMIT ttyChanged(this->tty());
    }
}

QStringList CreateExecInstanceJob::env() const
{
    Q_D(const CreateExecInstanceJob);
    return d->env;
}

void CreateExecInstanceJob::setEnv(const QStringList &env)
{
    Q_D(CreateExecInstanceJob);
    if (d->env != env) {
        qCDebug(schCore) << "Changing \"env\" from" << d->env << "to" << env;
        d->env = env;
        Q_EMIT envChanged(this->env());
    }
}

void CreateExecInstanceJob::addEnv(const QString &env)
{
    Q_D(CreateExecInstanceJob);
    qCDebug(schCore) << "Adding" << env << "to the list of environment variables";
    d->env.append(env);
    Q_EMIT envChanged(this->env());
}

void CreateExecInstanceJob::addEnv(const QString &key, const QString &value)
{
    const QString env = key + QLatin1Char('=') + value;
    addEnv(env);
}

void CreateExecInstanceJob::removeEnv(const QString &env)
{
    Q_D(CreateExecInstanceJob);
    qCDebug(schCore) << "Removing" << env << "from the list of environment variables";
    d->env.removeAll(env);
    Q_EMIT envChanged(this->env());
}

QStringList CreateExecInstanceJob::cmd() const
{
    Q_D(const CreateExecInstanceJob);
    return d->cmd;
}

void CreateExecInstanceJob::setCmd(const QStringList &cmd)
{
    Q_D(CreateExecInstanceJob);
    if (d->cmd != cmd) {
        qCDebug(schCore) << "Changing \"cmd\" from" << d->cmd << "to" << cmd;
        d->cmd = cmd;
        Q_EMIT cmdChanged(this->cmd());
    }
}

bool CreateExecInstanceJob::privileged() const
{
    Q_D(const CreateExecInstanceJob);
    return d->privileged;
}

void CreateExecInstanceJob::setPrivileged(bool privileged)
{
    Q_D(CreateExecInstanceJob);
    if (d->privileged != privileged) {
        qCDebug(schCore) << "Changing \"privileged\" from" << d->privileged << "to" << privileged;
        d->privileged = privileged;
        Q_EMIT privilegedChanged(this->privileged());
    }
}

QString CreateExecInstanceJob::user() const
{
    Q_D(const CreateExecInstanceJob);
    return d->user;
}

void CreateExecInstanceJob::setUser(const QString &user)
{
    Q_D(CreateExecInstanceJob);
    if (d->user != user) {
        qCDebug(schCore) << "Changing \"user\" from" << d->user << "to" << user;
        d->user = user;
        Q_EMIT userChanged(this->user());
    }
}

QString CreateExecInstanceJob::workingDir() const
{
    Q_D(const CreateExecInstanceJob);
    return d->workingDir;
}

void CreateExecInstanceJob::setWorkingDir(const QString &workingDir)
{
    Q_D(CreateExecInstanceJob);
    if (d->workingDir != workingDir) {
        qCDebug(schCore) << "Changing \"workingDir\" from" << d->workingDir << "to" << workingDir;
        d->workingDir = workingDir;
        Q_EMIT workingDirChanged(this->workingDir());
    }
}

#include "moc_createexecinstancejob.cpp"
