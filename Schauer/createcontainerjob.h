/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_CREATECONTAINERJOB_H
#define SCHAUER_CREATECONTAINERJOB_H

#include "schauer_global.h"
#include "job.h"

namespace Schauer {

class CreateContainerJobPrivate;

/*!
 * \ingroup api-jobs-containers
 * \brief Creates a new container from an image.
 *
 * Use this class to create a new container from a Docker image.
 *
 * \par API route
 * /containers/create
 *
 * \par API method
 * POST
 *
 * \dockerAPI{ContainerCreate}
 *
 * \sa RemoveContainerJob, StartContainerJob, StopContainerJob
 *
 * \headerfile "" <Schauer/CreateContainerJob>
 */
class SCHAUER_LIBRARY CreateContainerJob : public Job
{
    Q_OBJECT
    /*!
     * \brief Sets the name of the container to create.
     *
     * Must match <pre>/?[a-zA-Z0-9][a-zA-Z0-9_.-]+</pre>
     *
     * \par Access functions
     * \li QString name() const
     * \li void setName(const QString &name)
     *
     * \par Notifier signal
     * \li void nameChanged(const QString &name)
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    /*!
     * \brief Sets the configuration for the new container.
     *
     * At least the \a Image key must have a valid value.
     *
     * \par Access functions
     * \li QVariantHash containerConfig() const
     * \li void setContainerConfig(const QVariantHash &containerConfig)
     *
     * \par Notifier signal
     * \li void containerConfigChanged(const QVariantHash &containerConfig)
     */
    Q_PROPERTY(QVariantHash containerConfig READ containerConfig WRITE setContainerConfig NOTIFY containerConfigChanged)
public:
    /*!
     * \brief Contstructs a new %CreateContainerJob object with the given \a parent.
     */
    explicit CreateContainerJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %CreateContainerJob object.
     */
    ~CreateContainerJob() override;

    /*!
     * \brief Start creating a new Docker container asynchronously.
     *
     * When the job is finished, result() will be emitted.
     * To create a container in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link CreateContainerJob::name name\endlink property.
     * \sa setName(), nameChanged()
     */
    QString name() const;

    /*!
     * \brief Setter function for the \link CreateContainerJob::name name\endlink property.
     * \sa name(), nameChanged()
     */
    void setName(const QString &name);

    /*!
     * \brief Getter function for the \link CreateContainerJob::containerConfig containerConfig\endlink property.
     * \sa setContainerConfig(), containerConfigChanged()
     */
    QVariantHash containerConfig() const;

    /*!
     * \brief Setter function for the \link CreateContainerJob::containerConfig containerConfig\endlink property.
     * \sa containerConfig(), containerConfigChanged()
     */
    void setContainerConfig(const QVariantHash &containerConfig);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link CreateContainerJob::name name\endlink property.
     * \sa name(), setName()
     */
    void nameChanged(const QString &name);

    /*!
     * \brief Notifier signal for the \link CreateContainerJob::containerConfig containerConfig\endlink property.
     * \sa containerConfig(), setContainerConfig()
     */
    void containerConfigChanged(const QVariantHash &containerConfig);

private:
    Q_DECLARE_PRIVATE_D(s_ptr, CreateContainerJob)
    Q_DISABLE_COPY(CreateContainerJob)
};

}

#endif // SCHAUER_CREATECONTAINERJOB_H
