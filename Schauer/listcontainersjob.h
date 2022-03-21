/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_LISTCONTAINERSJOB_H
#define SCHAUER_LISTCONTAINERSJOB_H

#include "schauer_exports.h"
#include "job.h"

namespace Schauer {

/*!
 * \ingroup api-jobs
 * \defgroup api-jobs-containers Containers
 * \brief Job classes to handle and modify Docker containers
 */

class ListContainersJobPrivate;

/*!
 * \ingroup api-jobs-containers
 * \brief Queries the list of available containers form the Docker daemon.
 *
 * Use this class to get a list of available containers from the Docker daemon.
 * There is also AbstractContainersModel to provide this information.
 *
 * \par API route
 * /containers/json
 *
 * \par API method
 * GET
 *
 * \dockerAPI{ContainerList}
 *
 * \todo Implement filters
 *
 * \headerfile "" <Schauer/ListConstainersJob>
 */
class SCHAUER_LIBRARY ListContainersJob : public Job
{
    Q_OBJECT
    /*!
     * \brief Sets the number of most recently created containers to return, including non-running ones.
     *
     * By default this is set to \c 0 so that no limit will be set.
     *
     * \par Access functions
     * \li int limit() const
     * \li void setLimit(int limit)
     *
     * \par Notifier signal
     * \li void limitChanged(int limit)
     */
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    /*!
     * \brief Set this to \c true to show all containers.
     *
     * By default this is \c false and only running containers are shown.
     *
     * \par Access functions
     * \li bool showAll() const
     * \li void setShowAll(bool showAll)
     *
     * \par Notifier signal
     * \li void showAllChanged(bool showAll)
     */
    Q_PROPERTY(bool showAll READ showAll WRITE setShowAll NOTIFY showAllChanged)
    /*!
     * \brief Set this to \c true to return size of the container.
     *
     * The size will be shown in the fields \a SizeRw and \a SizeRootFs.
     * By default this is set to \c false.
     *
     * \par Access function
     * \li bool showSize() const
     * \li void setShowSize(bool showSize)
     *
     * \par Notifier signal
     * \li void showSizeChanged(bool showSize)
     */
    Q_PROPERTY(bool showSize READ showSize WRITE setShowSize NOTIFY showSizeChanged)
public:
    /*!
     * \brief Constructs a new %ListContainersJob with the given \a parent.
     */
    explicit ListContainersJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %LIstContainersJob.
     */
    ~ListContainersJob() override;

    /*!
     * \brief Starts querying the list of available containers asynchronously.
     *
     * When the job is finished, result() will be emitted.
     * To query the images in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link ListContainersJob::limit limit\endlink property.
     * \sa setLimit(), limitChanged()
     */
    int limit() const;

    /*!
     * \brief Setter function for the \link ListContainersJob::limit limit\endlink property.
     * \sa limit(), limitChanged()
     */
    void setLimit(int limit);

    /*!
     * \brief Getter function for the \link ListContainersJob::showAll showAll\endlink property.
     * \sa setShowAll(), showAllChanged()
     */
    bool showAll() const;

    /*!
     * \brief Setter function for the \link ListContainersJob::showAll showAll\endlink property.
     * \sa showAll(), showAllChanged()
     */
    void setShowAll(bool showAll);

    /*!
     * \brief Getter function for the \link ListContainersJob::showSize showSize\endlink property.
     * \sa setShowSize(), showSizeChanged()
     */
    bool showSize() const;

    /*!
     * \brief Setter function for the \link ListContainersJob::showSize showSize\endlink property.
     * \sa showSize(), showSizeChanged()
     */
    void setShowSize(bool showSize);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link ListContainersJob::limit limit\endlink property.
     * \sa limit(), setLimit()
     */
    void limitChanged(int limit);

    /*!
     * \brief Notifier signal for the \link ListContainersJob::showAll showAll\endlink property.
     * \sa showAll(), setShowAll
     */
    void showAllChanged(bool showAll);

    /*!
     * \brief Notifier signal for the \link ListContainersJob::showSize showSize\endlink property.
     * \sa showSize(), setShowSize()
     */
    void showSizeChanged(bool showSize);

private:
    Q_DECLARE_PRIVATE_D(s_ptr, ListContainersJob)
    Q_DISABLE_COPY(ListContainersJob)
};

}

#endif // SCHAUER_LISTCONTAINERSJOB_H
