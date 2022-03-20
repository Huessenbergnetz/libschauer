/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_LISTIMAGESJOB_H
#define SCHAUER_LISTIMAGESJOB_H

#include "schauer_global.h"
#include "job.h"

namespace Schauer {

/*!
 * \ingroup api-jobs
 * \defgroup api-jobs-images Images
 * \brief Job classes to handle and modify Docker images.
 */

class ListImagesJobPrivate;

/*!
 * \ingroup api-jobs-images
 * \brief Queries the list of available images from the Docker daemon.
 *
 * Use this class to get a list of available images from the docker daemon.
 * There is also AbstractImageModel to provide this information.
 *
 * \par API route
 * /images/json
 *
 * \par API method
 * GET
 *
 * \dockerAPI{ImageList}
 *
 * \todo Implement filters
 *
 * \headerfile "" <Schauer/ListImagesJob>
 */
class SCHAUER_LIBRARY ListImagesJob : public Job
{
    Q_OBJECT
    /*!
     * \brief Set this to \c true to show all images.
     *
     * By default this is \c false and only images from a final layer (no children) are returned.
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
     * \brief Set this to \c true to include digest information in the repsone.
     *
     * By default this is \c false and the \a RepoDigests field is not returned for each image.
     *
     * \par Access function
     * \li bool showDigests() const
     * \li void setShowDigests(bool showDigests)
     *
     * \par Notifier signal
     * \li void showDigestsChanged(bool showDigests)
     */
    Q_PROPERTY(bool showDigests READ showDigests WRITE setShowDigests NOTIFY showDigestsChanged)
public:
    /*!
     * \brief Constructs a new %ListImagesJob object with the given \a parent.
     */
    explicit ListImagesJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %ListImagesJob object.
     */
    ~ListImagesJob() override;

    /*!
     * \brief Starts querying the list of available images asynchronously.
     *
     * When the job is finished, result() will be emitted.
     * To query the images in a synchronous way, use exec().
     */
    void start() override;

    /*!
     * \brief Getter function for the \link ListImagesJob::showAll showAll\endlink property.
     * \sa setShowAll(), showAllChanged()
     */
    bool showAll() const;

    /*!
     * \brief Setter function for the \link ListImagesJob::showAll showAll\endlink property.
     * \sa showAll(), showAllChanged()
     */
    void setShowAll(bool showAll);

    /*!
     * \brief Getter function for the \link ListImagesJob::showDigests showDigests\endlink property.
     * \sa setShowDigests(), showDigestsChanged()
     */
    bool showDigests() const;

    /*!
     * \brief Setter function for the \link ListImagesJob::showDigests showDigests\endlink property.
     * \sa showDigests(), showDigestsChanged()
     */
    void setShowDigests(bool showDigests);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link ListImagesJob::showAll showAll\endlink property.
     * \sa showAll(), setShowAll()
     */
    void showAllChanged(bool showAll);

    /*!
     * \brief Notifier signal for the \link ListImagesJob::showDigests showDigests\endlink property.
     * \sa showDigests(), setShowDigests()
     */
    void showDigestsChanged(bool showDigests);

private:
    Q_DECLARE_PRIVATE_D(s_ptr, ListImagesJob)
    Q_DISABLE_COPY(ListImagesJob)
};

}

#endif // SCHAUER_LISTIMAGESJOB_H
