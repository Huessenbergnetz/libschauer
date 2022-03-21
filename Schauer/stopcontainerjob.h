/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_STOPCONTAINERJOB_H
#define SCHAUER_STOPCONTAINERJOB_H

#include "schauer_exports.h"
#include "job.h"

namespace Schauer {

class StopContainerJobPrivate;

/*!
 * \ingroup api-jobs-containers
 * \brief Stops a Docker container.
 *
 * Use this class to stop a previously \link StartContainerJob started\endlink Docker
 * container identified by its \link StopContainerJob::id id\endlink.
 *
 * \par API route
 * /containers/{\link StopContainerJob::id id\endlink}/stop
 *
 * \par API method
 * POST
 *
 * \dockerAPI{ContainerCreate}
 *
 * \sa CreateContainerJob, RemoveContainerJob, StartContainerJob
 *
 * \headerfile "" <Schauer/StopContainerJob>
 */
class SCHAUER_LIBRARY StopContainerJob : public Job
{
    Q_OBJECT
    /*!
     * \brief Sets the ID of the container to stop.
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
     * \brief Number of seconds to wait before killing the container.
     *
     * \par Access functions
     * \li int timeout() const
     * \li void setTimeout(int timeout)
     *
     * \par Notifier signal
     * \li void timeoutChanged(int timeout)
     */
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
public:
    /*!
     * \brief Constructs a new %StopContainerJob object with the given \a parent.
     */
    explicit StopContainerJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %StopContainerJob object.
     */
    ~StopContainerJob() override;

    /*!
     * \brief Start stopping the Docker container synchronously.
     *
     * When the job is finished, result() will be emitted.
     * To stop a container in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link StopContainerJob::id id\endlink property.
     * \sa setId(), idChanged()
     */
    QString id() const;

    /*!
     * \brief Setter function for the \link StopContainerJob::id id\endlink property.
     * \sa id(), idChanged()
     */
    void setId(const QString &id);

    /*!
     * \brief Getter function for the \link StopContainerJob::timeout timeout\endlink property.
     * \sa setTimeout(), timeoutChanged()
     */
    int timeout() const;

    /*!
     * \brief Setter function for the \link StopContainerJob::timeout timeout\endlink property.
     * \sa timeout(), timeoutChanged()
     */
    void setTimeout(int timeout);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link StopContainerJob::id id\endlink property.
     * \sa id(), setId()
     */
    void idChanged(const QString &id);

    /*!
     * \brief Notifier signal for the \link StopContainerJob::timeout timeout\endlink property.
     * \sa timeout(), setTimeout()
     */
    void timeoutChanged(int timeout);

private:
    Q_DECLARE_PRIVATE_D(s_ptr, StopContainerJob)
    Q_DISABLE_COPY(StopContainerJob)
};

}

#endif // SCHAUER_STOPCONTAINERJOB_H
