/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_CREATEEXECINSTANCEJOB_H
#define SCHAUER_CREATEEXECINSTANCEJOB_H

#include "schauer_exports.h"
#include "job.h"

namespace Schauer {

/*!
 * \ingroup api-jobs
 * \defgroup api-jobs-exec Exec
 * \brief Job classes to run new commands inside running containers.
 *
 * To execute a command in a container, you first need to \link CreateExecInstanceJob create an exec instance\endlink,
 * then \link StartExecInstanceJob start it\endlink.
 */

class CreateExecInstanceJobPrivate;

/*!
 * \ingroup api-jobs-exec
 * \brief Create an execution instance.
 *
 * After creating an execution instance, use the returned \a Id to start the
 * execution instance with the StartExecInstanceJob.
 *
 * Have a look at the description of the Job class to learn how to use Job
 * classes.
 *
 * \par Example
 * \code{.cpp}
 * // performing instance creation and start in a synchronous way
 * auto createExec = new CreateExecInstanceJob();
 * createExec->setId(QStringLiteral("my-container"));
 * createExec->setCmd(QStringList({QStringLiteral("mkdir"), QStringLiteral("-p"), QStringLiteral("/my/new/directory")}));
 * createExec->exec();
 *
 * const QString execId = createExec->replyData().object().value(QLatin1String("Id)).toString();
 *
 * auto startExec = new StartExecInstanceJob();
 * startExec->setId(execId);
 * startExec->exec();
 * \endcode
 *
 * \par API route
 * /container/{\link CreateExecInstanceJob::id id\endlink}/exec
 *
 * \par API method
 * POST
 *
 * \dockerAPI{ContainerExec}
 *
 * \headerfile "" <Schauer/CreateExecInstanceJob>
 */
class SCHAUER_LIBRARY CreateExecInstanceJob : public Job
{
    Q_OBJECT
    /*!
     * \brief ID or name of the container to exectue the \link CreateExecInstanceJob::cmd command\endlink in.
     *
     * By default this property holds an empty string. This property must be set to
     * a valid container ID or name to execute the job.
     *
     * \par Access functions
     * \li QString() id() const
     * \li void setId(const QString &id)
     *
     * \par Notifier signal
     * \li void idChanged(const QString &id)
     */
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    /*!
     * \brief Set this to \c true to attach to \a stdin of the exec command.
     *
     The default value is \c false.
     *
     * \par Access functions
     * \li bool attachStdin() const;
     * \li void setAttachStdin(bool attachStdin)
     *
     * \par Notifier signal
     * \li void attachStdinChanged(bool attachStdin)
     */
    Q_PROPERTY(bool attachStdin READ attachStdin WRITE setAttachStdin NOTIFY attachStdinChanged)
    /*!
     * \brief Set this to \c true to attach to \a stdout of the exec command.
     *
     * The default value is \c false.
     *
     * \par Access functions
     * \li bool attachStdout() const;
     * \li void setAttachStdout(bool attachStdout)
     *
     * \par Notifier signal
     * \li void attachStdoutChanged(bool attachStdout)
     */
    Q_PROPERTY(bool attachStdout READ attachStdout WRITE setAttachStdout NOTIFY attachStdoutChanged)
    /*!
     * \brief Set this to \c true to attach to \a stderr of the exec command.
     *
     * The default value is \c false.
     *
     * \par Access functions
     * \li bool attachStderr() const;
     * \li void setAttachStderr(bool attachStderr)
     *
     * \par Notifier signal
     * \li void attachStderrChanged(bool attachStderr)
     */
    Q_PROPERTY(bool attachStderr READ attachStderr WRITE setAttachStderr NOTIFY attachStderrChanged)
    /*!
     * \brief This property overrides the key sequence for detaching a container.
     *
     * Format is a singla character \c a-Z or \c ctrl-&lsaquo;value&rsaquo; where \c &lsaquo;value&rsaquo; is one of:
     * \c a-z, \c @, \c ^, \c [, \c _ or \c , . It can set multiple sequences by concatenating them separated by comm.
     * By default this property holds an empty string.
     *
     * \par Example
     * \code{.cpp}
     * auto job = new CreateExecInstanceJob();
     * job->setDetachKeys(QStringLiteral("ctrl-p,ctrl-q"));
     * \endcode
     *
     * \par Access functions
     * \li QString detachKeys() const
     * \li void setDetachKeys(const QString &detachKeys)
     *
     * \par Notifier signal
     * \li void detachKeysChanged(const QString &detachKeys)
     */
    Q_PROPERTY(QString detachKeys READ detachKeys WRITE setDetachKeys NOTIFY detachKeysChanged)
    /*!
     * \brief This property holds whether a pseudo-TTY should be allocated.
     *
     * Set it to \c true to allocate a pseudo-TTY. The default value is \c false.
     *
     * \par Access functions
     * \li bool tty() const
     * \li void setTty(bool tty)
     *
     * \par Notifier signal
     * \li void ttyChanged(bool tty)
     */
    Q_PROPERTY(bool tty READ tty WRITE setTty NOTIFY ttyChanged)
    /*!
     * \brief Sets the environment variabels used when executing the \link CreateExecInstanceJob::cmd command\endlink.
     *
     * The default value is an empty list. Set the list of environment variables in the form
     * \c QStringList({QStringLiteral("VAR=value"), …}).
     *
     * \par Access functions
     * \li QStringList env() const
     * \li void setEnv(const QStringList &env)
     * \li void addEnv(const QString &env)
     * \li void addEnv(const QString &key, const QString &value)
     * \li void removeEnv(const QString &env)
     *
     * \par Notifier signal
     * \li void envChanged(const QStringList &env)
     */
    Q_PROPERTY(QStringList env READ env WRITE setEnv NOTIFY envChanged)
    /*!
     * \brief Sets the command and its arguments to execute.
     *
     * The command is set as list of command and arguments. The default
     * value is an empty list and the job will not execute with an empty list.
     *
     * \par Example
     * \code{.cpp}
     * auto job = new CreateExecInstanceJob();
     * job->setCmd(QStringList({QStringLiteral("mkdir"), QStringLiteral("-p"), QStringLiteral("/my/new/directory")}));
     * \endcode
     *
     * \par Access functions
     * \li QStringList cmd() const
     * \li void setCmd(const QStringList &cmd)
     *
     * \par Notifier signal
     * \li void cmdChanged(const QStringList &cmd)
     */
    Q_PROPERTY(QStringList cmd READ cmd WRITE setCmd NOTIFY cmdChanged)
    /*!
     * \brief Set this to \c true to run the exec process with exended privileges.
     *
     * The default value is \c false.
     *
     * \par Access functions
     * \li bool privileged() const
     * \li void setPrivileged(bool privileged)
     *
     * \par Notifier signal
     * \li void privilegedChanged(bool privileged)
     */
    Q_PROPERTY(bool privileged READ privileged WRITE setPrivileged NOTIFY privilegedChanged)
    /*!
     * \brief This property holds the user, and optionally, group to run the exec process
     * inside the container.
     *
     * Format is one of \c user, \c user:group, \c uid or \c uid:gid.
     *
     * \par Access functions
     * \li QString user() const
     * \li void setUser(const QString &user)
     *
     * \par Notifier signal
     * \li void userChanged(const QString &user)
     */
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    /*!
     * \brief This property holds the working directoy for the exec process inside the container.
     *
     * \par Access functions
     * \li QString workingDir() const
     * \li void setWorkingDir(const QString &workingDir)
     *
     * \par Notifier signal
     * \li void workingDirChanged(const QString &workingDir)
     */
    Q_PROPERTY(QString workingDir READ workingDir WRITE setWorkingDir NOTIFY workingDirChanged)
public:
    /*!
     * \brief Constructs a new %CreateExecInstanceJob with the given \a parent.
     */
    explicit CreateExecInstanceJob(QObject *parent = nullptr);

    /*!
     * Destroys the %CreateExecInstanceJob object.
     */
    ~CreateExecInstanceJob() override;

    /*!
     * \brief Start creating an execution instance asynchronously.
     *
     * When the job is finished, result() is emitted.
     * To create an execution instance in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::id id\endlink property.
     * \sa setId(), idChanged()
     */
    QString id() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::id id\endlink property.
     * \sa id(), idChanged()
     */
    void setId(const QString &id);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::attachStdin attachStdin\endlink property.
     * \sa setAttachStdin(), attachStdinChanged()
     */
    bool attachStdin() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::attachStdin attachStdin\endlink property.
     * \sa attachStdin(), attachStdinChanged()
     */
    void setAttachStdin(bool attachStdin);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::attachStdout attachStdout\endlink property.
     * \sa setAttachStdout(), attachStdoutChanged()
     */
    bool attachStdout() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::attachStdout attachStdout\endlink property.
     * \sa attachStdout(), attachStdoutChanged()
     */
    void setAttachStdout(bool attachStdout);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::attachStderr attachStderr\endlink property.
     * \sa setAttachStderr(), attachStderrChanged()
     */
    bool attachStderr() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::attachStderr attachStderr\endlink property.
     * \sa attachStderr(), attachStderrChanged()
     */
    void setAttachStderr(bool attachStderr);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::detachKeys detachKeys\endlink property.
     * \sa setDetachKeys(), detachKeysChanged()
     */
    QString detachKeys() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::detachKeys detachKeys\endlink property.
     * \sa detachKeys(), detachKeysChanged()
     */
    void setDetachKeys(const QString &detachKeys);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::tty tty\endlink property.
     * \sa setTty(), ttyChanged()
     */
    bool tty() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::tty tty\endlink property.
     * \sa tty(), ttyChanged()
     */
    void setTty(bool tty);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::tty tty\endlink property.
     * \sa setEnv(), envChanged(), addEnv(), removeEnv()
     */
    QStringList env() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::tty tty\endlink property.
     *
     * Set the list of environment variables in the form \c QStringList({QStringLiteral("VAR=value"), …}).
     *
     * \sa env(), envChanged(), addEnv(), removeEnv()
     */
    void setEnv(const QStringList &env);

    /*!
     * \brief Adds a single key-value-pair to the \link CreateExecInstanceJob::env list of environment variables\endlink.
     *
     * \par Example
     * \code{.cpp}
     * auto job = new CreateExecInstanceJob();
     * job->addEnv(QStringLiteral("FOO=bar"));
     * \endcode
     *
     * \sa env(), setEnv(), envChanged(), removeEnv();
     */
    void addEnv(const QString &env);

    /*!
     * \brief Adds \a key and \a value as pair to the \link CreateExecInstanceJob::env list of environment variables\endlink.
     *
     * \par Example
     * \code{.cpp}
     * auto job = new CreateExecInstanceJob();
     * job->addEnv(QStringLiteral("FOO"), QStringLiteral("bar"));
     * \endcode
     *
     * \sa env(), setEnv(), envChanged(), removeEnv();
     */
    void addEnv(const QString &key, const QString &value);

    /*!
     * \brief Removes \a env from the \link CreateExecInstanceJob::env list of environment variables\endlink.
     *
     * \par Example
     * \code{.cpp}
     * auto job = new CreateExecInstanceJob();
     * job->removeEnv(QStringLiteral("FOO"), QStringLiteral("bar"));
     * \endcode
     *
     * \sa env(), setEnv(), envChanged(), addEnv()
     */
    void removeEnv(const QString &env);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::cmd cmd\endlink property.
     * \sa setCmd(), cmdChanged()
     */
    QStringList cmd() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::cmd cmd\endlink property.
     * \sa setCmd(), cmdChanged()
     */
    void setCmd(const QStringList &cmd);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::privileged privileged\endlink property.
     * \sa setPrivileged(), privilegedChanged()
     */
    bool privileged() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::privileged privileged\endlink property.
     * \sa privileged(), privilegedChanged()
     */
    void setPrivileged(bool privileged);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::user user\endlink property.
     * \sa setUser(), userChanged()
     */
    QString user() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::user user\endlink property.
     * \sa user(), userChanged()
     */
    void setUser(const QString &user);

    /*!
     * \brief Getter function for the \link CreateExecInstanceJob::workingDir workingDir\endlink property.
     * \sa setWorkingDir(), workingDirChanged()
     */
    QString workingDir() const;

    /*!
     * \brief Setter function for the \link CreateExecInstanceJob::workingDir workingDir\endlink property.
     * \sa workingDir(), workingDirChanged()
     */
    void setWorkingDir(const QString &workingDir);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::id id\endlink property.
     * \sa id(), setId()
     */
    void idChanged(const QString &id);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::attachStdin attachStdin\endlink property.
     * \sa attachStdin(), setAttachStdin()
     */
    void attachStdinChanged(bool attachStdin);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::attachStdout attachStdout\endlink property.
     * \sa attachStdout(), setAttachStdout()
     */
    void attachStdoutChanged(bool attachStdout);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::attachStderr attachStderr\endlink property.
     * \sa attachStderr(), setAttachStderr()
     */
    void attachStderrChanged(bool attachStderr);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::detachKeys detachKeys\endlink property.
     * \sa detachKeys(), setDetachKeys()
     */
    void detachKeysChanged(const QString &detachKeys);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::tty tty\endlink property.
     * \sa tty(), setTty()
     */
    void ttyChanged(bool tty);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::env env\endlink property.
     * \sa env(), setEnv()
     */
    void envChanged(const QStringList &env);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::cmd cmd\endlink property.
     * \sa cmd(), setCmd()
     */
    void cmdChanged(const QStringList &cmd);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::privileged privileged\endlink property.
     * \sa privileged(), setPrivileged()
     */
    void privilegedChanged(bool privileged);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::user user\endlink property.
     * \sa user(), setUser()
     */
    void userChanged(const QString &user);

    /*!
     * \brief Notifier signal for the \link CreateExecInstanceJob::workingDir workingDir\endlink property.
     * \sa workingDir(), setWorkingDir()
     */
    void workingDirChanged(const QString &workingDir);

private:
    Q_DISABLE_COPY(CreateExecInstanceJob)
    Q_DECLARE_PRIVATE_D(s_ptr, CreateExecInstanceJob)
};

}

#endif // SCHAUER_CREATEEXECINSTANCEJOB_H
