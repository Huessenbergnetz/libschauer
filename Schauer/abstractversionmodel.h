/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTVERSIONMODEL_H
#define SCHAUER_ABSTRACTVERSIONMODEL_H

#include "schauer_exports.h"
#include "abstractbasemodel.h"
#include <QDateTime>

namespace Schauer {

class AbstractVersionModelPrivate;

/*!
 * \ingroup data-models
 * \brief Base class for models providing version information.
 *
 * The properties belonging directly to this model describe the Docker daemon itself.
 * The model content describes subcomponents of the Docker daemon like the engine, containerd,
 * etc.
 *
 * \sa VersionListModel
 */
class SCHAUER_LIBRARY AbstractVersionModel : public AbstractBaseModel
{
    Q_OBJECT
    /*!
     * \brief API version of the Docker daemon.
     *
     * \par Access functions
     * \li QString apiVersion() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString apiVersion READ apiVersion NOTIFY loaded)
    /*!
     * \brief Architecture the Docker daemon runs on.
     *
     * \par Access functions
     * \li QString arch() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString arch READ arch NOTIFY loaded)
    /*!
     * \brief Build time of the Docker daemon.
     *
     * \par Access functions
     * \li QDateTime buildTime() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QDateTime buildTime READ buildTime NOTIFY loaded)
    /*!
     * \brief Git commit hash of the Docker daemon.
     *
     * \par Access functions
     * \li QString gitCommit() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString gitCommit READ gitCommit NOTIFY loaded)
    /*!
     * \brief Go version the Docker daemon has been built with.
     *
     * \par Access functions
     * \li QString goVersion() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString goVersion READ goVersion NOTIFY loaded)
    /*!
     * \brief Kernel version of the host system the Docker daemon runs on.
     *
     * \par Access functions
     * \li QString kernelVersion() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString kernelVersion READ kernelVersion NOTIFY loaded)
    /*!
     * \brief The minimum API version supported by this Docker daemon.
     *
     * \par Access functions
     * \li QString minApiVersion() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString minApiVersion READ minApiVersion NOTIFY loaded)
    /*!
     * \brief The host operating system the Docker daemon runs on.
     *
     * \par Access functions
     * \li QString os() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString os READ os NOTIFY loaded)
    /*!
     * \brief Name of the platform the Docker daemon runs on.
     *
     * \par Access functions
     * \li QString platformName() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString platformName READ platformName NOTIFY loaded)
    /*!
     * \brief Version of the Docker daemon.
     *
     * \par Access functions
     * \li QString version() const
     *
     * \par Notifier signal
     * \li void loaded()
     */
    Q_PROPERTY(QString version READ version NOTIFY loaded)
public:
    /*!
     * \brief Constructs a new %AbstractVersionModel with the given \a parent.
     */
    explicit AbstractVersionModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractVersionModel object.
     */
    ~AbstractVersionModel() override;

    /*!
     * \brief Returns the API version of the Docker daemon.
     */
    QString apiVersion() const;
    /*!
     * \brief Returns the architecture the Docker daemon runs on.
     */
    QString arch() const;
    /*!
     * \brief Returns the build time of the Docker daemon.
     */
    QDateTime buildTime() const;
    /*!
     * \brief Returns the Git commit of the Docker daemon.
     */
    QString gitCommit() const;
    /*!
     * \brief Returns the Go version the Docker daemon has been built with.
     */
    QString goVersion() const;
    /*!
     * \brief Returns the kernel version of the host system the Docker daemon runs on.
     */
    QString kernelVersion() const;
    /*!
     * \brief Returns the minimum API version supported by this Docker daemon.
     */
    QString minApiVersion() const;
    /*!
     * \brief Returns the host operating systemd the Docker daemon runs on.
     */
    QString os() const;
    /*!
     * \brief Returns the name of the platform the Docker daemon runs on.
     */
    QString platformName() const;
    /*!
     * \brief Returns the version of the Docker daemon.
     */
    QString version() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    AbstractVersionModel(AbstractVersionModelPrivate &dd, QObject *parent = nullptr);

    void clear() override;

private:
    Q_DECLARE_PRIVATE_D(s_ptr, AbstractVersionModel)
    Q_DISABLE_COPY(AbstractVersionModel)
};

}

#endif // SCHAUER_ABSTRACTVERSIONMODEL_H
