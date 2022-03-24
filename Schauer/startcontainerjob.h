/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_STARTCONTAINERJOB_H
#define SCHAUER_STARTCONTAINERJOB_H

#include "schauer_exports.h"
#include "job.h"

namespace Schauer {

class StartContainerJobPrivate;

/*!
 * \ingroup api-jobs-containers
 * \brief Starts a Docker container.
 *
 * Use this class to start a previously \link CreateContainerJob created\endlink container
 * identified by its \link StartContainerJob::id id\endlink.
 *
 * \par API route
 * /containers/{\link StartContainerJob::id id\endlink}/stop
 *
 * \par API method
 * POST
 *
 * \dockerAPI{ContainerStart}
 *
 * \sa CreateContainerJob, RemoveContainerJob, StopContainerJob
 *
 * \headerfile "" <Schauer/StartContainerJob>
 */
class SCHAUER_LIBRARY StartContainerJob : public Job
{
    Q_OBJECT
    /*!
     * \brief Sets the ID of the container to start.
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
     * \brief Set this to override the key sequence for detaching a container.
     *
     * Allowed values are a single character \c a-Z or \c ctrl-&lsaquo;value&rsaquo; where \c &lsaquo;value&rsaquo;
     * is one of \c a-z, \c @, \c ^, \c [, \c _ or \c , . It can set multiple sequences by concatenating them separated by comm.
     * By default this property holds an empty string.
     *
     * \par Example
     * \code{.cpp}
     * auto job = new StartContainerJob();
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
public:
    /*!
     * \brief Constructs a new %StartContainerJob with the given \a parent.
     */
    explicit StartContainerJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %StartContainerJob object.
     */
    ~StartContainerJob() override;

    /*!
     * \brief Starts the container identified by \link StartContainerJob::id id\endlink asynchronously.
     *
     * When the job is finished, result() will be emitted.
     * To start a container in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link StartContainerJob::id id\endlink property.
     * \sa setId(), idChanged()
     */
    QString id() const;

    /*!
     * \brief Setter function for the \link StartContainerJob::id id\endlink property.
     * \sa id(), idChanged()
     */
    void setId(const QString &id);

    /*!
     * \brief Getter function for the \link StartContainerJob::detachKeys detachKeys\endlink property.
     * \sa setDetachKeys(), detachKeysChanged()
     */
    QString detachKeys() const;

    /*!
     * \brief Setter function for the \link StartContainerJob::detachKeys detachKeys\endlink property.
     * \sa detachKeys(), detachKeysChanged()
     */
    void setDetachKeys(const QString &detachKeys);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link StartContainerJob::id id\endlink property.
     * \sa id(), setId()
     */
    void idChanged(const QString &id);

    /*!
     * \brief Notifier signal for the \link StartContainerJob::detachKeys detachKeys\endlink property.
     * \sa detachKeys(), setDetachKeys()
     */
    void detachKeysChanged(const QString &detachKeys);

private:
    Q_DECLARE_PRIVATE_D(s_ptr, StartContainerJob)
    Q_DISABLE_COPY(StartContainerJob)
};

}

#endif // SCHAUER_STARTCONTAINERJOB_H
