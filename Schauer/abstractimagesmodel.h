/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTIMAGESMODEL_H
#define SCHAUER_ABSTRACTIMAGESMODEL_H

#include "schauer_global.h"
#include "abstractbasemodel.h"

namespace Schauer {

class AbstractImagesModelPrivate;

/*!
 * \ingroup data-models
 * \brief Base class for models providing a list or table of available images.
 *
 * This uses ListImagesJob to get a list of images from the docker daemon.
 *
 * See ImageListModel for a full implementation.
 */
class SCHAUER_LIBRARY AbstractImagesModel : public AbstractBaseModel
{
    Q_OBJECT
    /*!
     * \brief Set this to \c true to show all images.
     *
     * By default this is \c false and only images from a final layer (no children) are shown.
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
     * \brief Constructs a new %AbstractImagesModel with the given \a parent.
     */
    explicit AbstractImagesModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractImagesModel.
     */
    ~AbstractImagesModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Getter function for the \link AbstractImagesModel::showAll showAll\endlink property.
     * \sa setShowAll(), showAllChanged()
     */
    bool showAll() const;

    /*!
     * \brief Setter function for the \link AbstractImagesModel::showAll showAll\endlink property.
     * \sa showAll(), showAllChanged()
     */
    void setShowAll(bool showAll);

    /*!
     * \brief Getter function for the \link AbstractImagesModel::showDigests showDigests\endlink property.
     * \sa setShowDigests(), showDigestsChanged()
     */
    bool showDigests() const;

    /*!
     * \brief Setter function for the \link AbstractImagesModel::showDigests showDigests\endlink property.
     * \sa showDigests(), showDigestsChanged()
     */
    void setShowDigests(bool showDigests);

    /*!
     * \brief Returns \c true if the model contains an image identfied by \a repo and/or \a tag.
     */
    bool containsRepoTag(const QString &repo, const QString &tag = QString()) const;

    /*!
     * \overload
     */
    bool containsRepoTag(QLatin1String repo, QLatin1String tag = QLatin1String()) const;

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link AbstractImagesModel::showAll showAll\endlink property.
     * \sa showAll(), setShowAll()
     */
    void showAllChanged(bool showAll);

    /*!
     * \brief Notifier signal for the \link AbstractImagesModel::showDigests showDigests\endlink property.
     * \sa showDigests(), setShowDigests()
     */
    void showDigestsChanged(bool schowDigests);

protected:
    AbstractImagesModel(AbstractImagesModelPrivate &dd, QObject *parent = nullptr);

    void clear() override;

private:
    Q_DECLARE_PRIVATE_D(s_ptr, AbstractImagesModel)
    Q_DISABLE_COPY(AbstractImagesModel)
};

}

#endif // SCHAUER_ABSTRACTIMAGESMODEL_H
