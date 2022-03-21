/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_REMOVECONTAINERJOB_H
#define SCHAUER_REMOVECONTAINERJOB_H

#include "schauer_exports.h"
#include "job.h"

namespace Schauer {

class RemoveContainerJobPrivate;

/*!
 * \ingroup api-jobs-containers
 * \brief Remove a container from the Docker daemon.
 *
 * Use this class to remove containers from the Docker daemon
 * identified by its \link RemoveContainerJob::id id\endlink.
 *
 * \par API route
 * /containers/create
 *
 * \par API method
 * POST
 *
 * \dockerAPI{ContainerCreate}
 *
 * \sa CreateContainerJob, StartContainerJob, StopContainerJob
 *
 * \headerfile "" <Schauer/CreateContainerJob>
 */
class SCHAUER_LIBRARY RemoveContainerJob : public Job
{
    Q_OBJECT
    /*!
     * \brief Sets the ID of the container to remove.
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
     * \brief Set this to \c true to remove anonymous volumes associated with the container.
     *
     * \par Access functions
     * \li bool removeAnonVolumes() const
     * \li void setRemoveAnonVolumes(bool removeAnonVolumes)
     *
     * \par Notifier signal
     * \li void removeAnonVolumnesChanged(bool removeAnonVolumes)
     */
    Q_PROPERTY(bool removeAnonVolumes READ removeAnonVolumes WRITE setRemoveAnonVolumes NOTIFY removeAnonVolumesChanged)
    /*!
     * \brief Set this to \c true to force removing the container.
     *
     * If the container is running, it will be killed before removing it.
     *
     * \par Access functions
     * \li bool force() const
     * \li void setForce(bool force)
     *
     * \par Notifier signal
     * \li void forceChanged(bool force)
     */
    Q_PROPERTY(bool force READ force WRITE setForce NOTIFY forceChanged)
    /*!
     * \brief Set this to \c true to remove links associated with the container.
     *
     * \par Access functions
     * \li bool removeLinks() const
     * \li void setRomveLinks(bool removeLinks)
     *
     * \par Nofifier signal
     * \li void removeLinksChanged(bool removeLinks)
     */
    Q_PROPERTY(bool removeLinks READ removeLinks WRITE setRemoveLinks NOTIFY removeLinksChanged)
public:
    /*!
     * \brief Constructs a new %RemoveContainerJob with the given \a parent.
     */
    explicit RemoveContainerJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %RemoveContainerJob object.
     */
    ~RemoveContainerJob() override;

    /*!
     * \brief Start removing the Docker container aswynchronously.
     *
     * When the job is finished, result() wil be emitted.
     * To remove a container in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link RemoveContainerJob::id id\endlink property.
     * \sa setId(), idChanged()
     */
    QString id() const;

    /*!
     * \brief Setter function for the \link RemoveContainerJob::id id\endlink property.
     * \sa id(), idChanged()
     */
    void setId(const QString &id);

    /*!
     * \brief Getter function for the \link RemoveContainerJob::removeAnonVolumes removeAnonVolumes\endlink property.
     * \sa setRemoveAnonVolumes(), removeAnonVolumesChanged()
     */
    bool removeAnonVolumes() const;

    /*!
     * \brief Setter function for the \link RemoveContainerJob::removeAnonVolumes removeAnonVolumes\endlink property.
     * \sa removeAnonVolumes(), removeAnonVolumesChanged()
     */
    void setRemoveAnonVolumes(bool removeAnonVolumes);

    /*!
     * \brief Getter function for the \link RemoveContainerJob::force force\endlink property.
     * \sa setForce(), forceChanged()
     */
    bool force() const;

    /*!
     * \brief Setter function for the \link RemoveContainerJob::force force\endlink property.
     * \sa force(), forceChanged()
     */
    void setForce(bool force);

    /*!
     * \brief Getter function for the \link RemoveContainerJob::removeLinks removeLinks\endlink property.
     * \sa setRemoveLinks(), removeLinksChanged()
     */
    bool removeLinks() const;

    /*!
     * \brief Setter function for the \link RemoveContainerJob::removeLinks removeLinks\endlink property.
     * \sa removeLinks(), removeLinksChanged()
     */
    void setRemoveLinks(bool removeLinks);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link RemoveContainerJob::id id\endlink property.
     * \sa id(), setId()
     */
    void idChanged(const QString &id);

    /*!
     * \brief Notifier signal for the \link RemoveContainerJob::removeAnonVolumes removeAnonVolumes\endlink property.
     * \sa removeAnonVolumes(), setRemoveAnonVolumes()
     */
    void removeAnonVolumesChanged(bool removeAnonVolumes);

    /*!
     * \brief Notifier signal for the \link RemoveContainerJob::force force\endlink property.
     * \sa force(), setForce()
     */
    void forceChanged(bool force);

    /*!
     * \brief Notifier signal for the \link RemoveContainerJob::removeLinks removeLinks\endlink property.
     * \sa removeLinks(), setRemoveLinks()
     */
    void removeLinksChanged(bool removeLinks);

private:
    Q_DECLARE_PRIVATE_D(s_ptr, RemoveContainerJob)
    Q_DISABLE_COPY(RemoveContainerJob)
};

}

#endif // SCHAUER_REMOVECONTAINERJOB_H
