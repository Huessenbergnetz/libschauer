/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTCONTAINERMODEL_H
#define SCHAUER_ABSTRACTCONTAINERMODEL_H

#include "schauer_exports.h"
#include "abstractbasemodel.h"

namespace Schauer {

class AbstractContainerModelPrivate;

/*!
 * \ingroup data-models
 * \brief Base class for models providing a list or table of available containers.
 *
 * This uses ListContainersJob to get a list of containers from the Docker daemon.
 *
 * See ContainerListModel for a full implementation.
 *
 * \todo Add data storage for container Ports, HostConfig, NetworkSettings and Mounts.
 */
class SCHAUER_LIBRARY AbstractContainerModel : public AbstractBaseModel
{
    Q_OBJECT
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
     * \brief Constructs a new %AbstractContainerModel object with the given \a parent.
     */
    explicit AbstractContainerModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractContainerModel object.
     */
    ~AbstractContainerModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Getter function for the \link AbstractContainerModel::showAll showAll\endlink property.
     * \sa setShowAll(), showAllChanged()
     */
    bool showAll() const;

    /*!
     * \brief Setter function for the \link AbstractContainerModel::showAll showAll\endlink property.
     * \sa showAll(), showAllChanged()
     */
    void setShowAll(bool showAll);

    /*!
     * \brief Getter function for the \link AbstractContainerModel::showSize showSize\endlink property.
     * \sa setShowSize(), showSizeChanged()
     */
    bool showSize() const;

    /*!
     * \brief Setter function for the \link AbstractContainerModel::showSize showSize\endlink property.
     * \sa showSize(), showSizeChanged()
     */
    void setShowSize(bool showSize);

    /*!
     * \brief Returns \c true if the model contains a container identified by \a idOrName.
     *
     * \a idOrName can be either a container ID or a container name.
     */
    bool contains(const QString &idOrName) const;

    /*!
     * \overload
     */
    bool contains(QLatin1String idOrName) const;

    /*!
     * \brief Returns \c true if the model contains a container that is based on \a image.
     */
    bool containsImage(const QString &image) const;

    /*!
     * \overload
     */
    bool containsImage(QLatin1String &image) const;

    /*!
     * \brief Returns \c true if the model contains a container that is based on \a imageId.
     */
    bool containsImageId(const QString &imageId) const;

    /*!
     * \overload
     */
    bool containsImageId(QLatin1String &imageId) const;

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link AbstractContainerModel::showAll showAll\endlink property.
     * \sa showAll(), setShowAll
     */
    void showAllChanged(bool showAll);

    /*!
     * \brief Notifier signal for the \link AbstractContainerModel::showSize showSize\endlink property.
     * \sa showSize(), setShowSize()
     */
    void showSizeChanged(bool showSize);

protected:
    AbstractContainerModel(AbstractContainerModelPrivate &dd, QObject *parent = nullptr);

    void clear() override;

private:
    Q_DECLARE_PRIVATE_D(s_ptr, AbstractContainerModel)
    Q_DISABLE_COPY(AbstractContainerModel)
};

}

#endif // SCHAUER_ABSTRACTCONTAINERMODEL_H
