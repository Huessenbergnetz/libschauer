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

/*!
 * \ingroup api-jobs-exec
 * \brief Start a previously set up exec instance.
 *
 * After creating an execution instance with CreateExecInstanceJob, use the
 * returned \a Id to start the execution instance with this class.
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
 * /exec/{\link StartExecInstanceJob::id id\endlink}/start
 *
 * \par API method
 * POST
 *
 * \dockerAPI{ExecStart}
 *
 * \headerfile "" <Schauer/StartExecInstanceJob>
 */
class SCHAUER_LIBRARY StartExecInstanceJob : public Job
{
    Q_OBJECT
    /*!
     * \brief This propery holds the ID of the execution instance to start.
     *
     * Get an execution instance ID by create a new execution instance with CreateExecInstanceJob.
     *
     * \par Access functions
     * \li QString id() const
     * \li void setId(const QString &id)
     *
     * \par Notifier signal
     * \li void idChanged(const QString &id)
     */
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    /*!
     * \brief This property holds whether to detach from the command.
     *
     * The default value is \c true.
     *
     * \par Access functions
     * \li bool detach() const
     * \li void setDetach(bool detach)
     *
     * \par Notifier signal
     * \li void detachChanged(bool detach)
     */
    Q_PROPERTY(bool detach READ detach WRITE setDetach NOTIFY detachChanged)
    /*!
     * \brief This property holds whether to allocate a pseudo-TTY.
     *
     * The default value is \c false.
     *
     * \par Access functions
     * \li bool tty() const
     * \li void setTty(bool tty)
     *
     * \par Notifier signal
     * \li void ttyChanged(bool tty)
     */
    Q_PROPERTY(bool tty READ tty WRITE setTty NOTIFY ttyChanged)
public:
    /*!
     * \brief Constructs a new %StartExecInstanceJob object with the given \a parent.
     */
    explicit StartExecInstanceJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %StartExecInstanceJob object.
     */
    ~StartExecInstanceJob() override;

    /*!
     * \brief Start an execution instance asynchronously.
     *
     * When the job is finished, result() is emitted.
     * To start an execution instance in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link StartExecInstanceJob::id id\endlink property.
     * \sa setId(), idChanged()
     */
    QString id() const;

    /*!
     * \brief Setter function for the \link StartExecInstanceJob::id id\endlink property.
     * \sa id(), idChanged()
     */
    void setId(const QString &id);

    /*!
     * \brief Getter function for the \link StartExecInstanceJob::detach detach\endlink property.
     * \sa setDetach(), detachChanged()
     */
    bool detach() const;

    /*!
     * \brief Setter function for the \link StartExecInstanceJob::detach detach\endlink property.
     * \sa detach(), detachChanged()
     */
    void setDetach(bool detach);

    /*!
     * \brief Getter function for the \link StartExecInstanceJob::tty tty\endlink property.
     * \sa setTty(), ttyChanged()
     */
    bool tty() const;

    /*!
     * \brief Setter function for the \link StartExecInstanceJob::tty tty\endlink property.
     * \sa tty(), ttyChanged()
     */
    void setTty(bool tty);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link StartExecInstanceJob::id id\endlink property.
     * \sa id(), setId()
     */
    void idChanged(const QString &id);

    /*!
     * \brief Notifier signal for the \link StartExecInstanceJob::detach detach\endlink property.
     * \sa detach(), setDetach()
     */
    void detachChanged(bool detach);

    /*!
     * \brief Notifier signal for the \link StartExecInstanceJob::tty tty\endlink property.
     * \sa tty(), setTty()
     */
    void ttyChanged(bool tty);

private:
    Q_DISABLE_COPY(StartExecInstanceJob)
    Q_DECLARE_PRIVATE_D(s_ptr, StartExecInstanceJob)
};

}

#endif // SCHAUER_STARTEXECINSTANCEJOB_H
