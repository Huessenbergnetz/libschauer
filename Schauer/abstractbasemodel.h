/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTBASEMODEL_H
#define SCHAUER_ABSTRACTBASEMODEL_H

#include "schauer_exports.h"
#include "abstractconfiguration.h"
#include <QAbstractItemModel>
#include <memory>

namespace Schauer {

/*!
 * \defgroup data-models Models
 * \brief Data models providing Docker REST API information.
 */

class AbstractBaseModelPrivate;

/*!
 * \ingroup data-models
 * \brief Base class for all data models.
 */
class SCHAUER_LIBRARY AbstractBaseModel : public QAbstractItemModel
{
    Q_OBJECT
    /*!
     * \brief Pointer to an object providing configuration data.
     *
     * This property holds a pointer to an object of a class derived from AbstractConfiguration
     * that provides configuration information for the API request. It especialls provides the
     * remote host and port. See AbstractConfiguration for more information.
     *
     * You can also set a global default configuration via Schauer::setDefaultConfiguration()
     * that will be used if no local configuration is available.
     *
     * \par Access functions
     * \li AbstractConfiguration *configuration() const
     * \li void setConfiguration(AbstractCofiguration *configuration)
     *
     * \par Notifier signal
     * \li void configurationChanged(AbstractConfiguration *configuration)
     */
    Q_PROPERTY(Schauer::AbstractConfiguration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    /*!
     * \brief Indicates loading state.
     *
     * This property holds \c true while the model is loading, otherwise \c false.
     *
     * \par Access functions
     * \li bool isLoading() const
     *
     * \par Notifier signal
     * \li void isLoadingChanged(bool isLoading)
     */
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
    /*!
     * \brief Error code.
     *
     * This property holds the error code after the model data hase been loaded.
     * If no error has occured, the error code will be \c 0.
     * \link AbstractBaseModel::errorString errorString\endlink property will hold
     * a human-readble error message.
     *
     * \par Access functions
     * \li int error() const
     *
     * \par Notifier signal
     * \li void errorChanged(int error)
     */
    Q_PROPERTY(int error READ error NOTIFY errorChanged)
    /*!
     * \brief Human-readable error message.
     *
     * This property holds a translated, human-readable description of the
     * \link AbstractBaseModel::error error\endlink.
     *
     * \par Access functions
     * \li QString error() const
     *
     * \par Notifier signal
     * \li void errorChanged(int error)
     */
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged)
public:
    /*!
     * \brief Constructs a new %AbstractBaseModel class with the given \a parent.
     */
    explicit AbstractBaseModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractBaseModel object.
     */
    ~AbstractBaseModel() override;

    /*!
     * \brief Load mode for model.
     */
    enum LoadMode : int {
        LoadAsync,  /**< Loads the model data asynchronously. */
        LoadSync    /**< Loads the model data synchronously. */
    };
    Q_ENUM(LoadMode)

    /*!
     * \brief Getter function for the \link AbstractBaseModel::configuration configuraion\endlink property.
     * \sa setConfiguration(), configurationChanged()
     */
    AbstractConfiguration* configuration() const;

    /*!
     * \brief Setter function for the \link AbstractBaseModel::configuration configuraion\endlink property.
     * \sa configuration(), configurationChanged()
     */
    void setConfiguration(AbstractConfiguration *configuration);

    /*!
     * \brief Returns \c true while the model is loading, otherwise returns \c false.
     */
    bool isLoading() const;

    /*!
     * \brief Returns the error code, if there has been an error.
     *
     * If there has been no error, \c 0 will be returned.
     *
     * \sa errorString()
     */
    int error() const;

    /*!
     * \brief Returns the human-readable error message.
     *
     * This provides a translated, human-readable description of the error().
     * Only call if error() is not \c 0.
     *
     * \sa error()
     */
    QString errorString() const;

public Q_SLOTS:
    /*!
     * \brief Loads the model data.
     *
     * Call this function to load the model data either in asynchronous or
     * synchronous \a mode. When executing load, \link AbstractBaseModel::isLoading isLoading\endlink
     * property will be \c true while the model loads its data. After data loading has been finished,
     * \link AbstractBaseModel::isLoading isLoading\endlink will be \c false and the loaded() signal
     * will be emitted.
     *
     * Use the \link AbstractBaseModel::error error\endlink property to check if an error has occured.
     */
    bool load(Schauer::AbstractBaseModel::LoadMode mode = LoadAsync);

Q_SIGNALS:
    /*!
     * \brief Emitted to indicate model data loading has been finished.
     *
     * Use error() to check if no error has been occured.
     */
    void loaded();
    /*!
     * \brief Notifier signal for the \link AbstractBaseModel::isLoading isLoading\endlink property.
     * \sa isLoading()
     */
    void isLoadingChanged(bool isLoading);
    /*!
     * \brief Notifier signal for the \link AbstractBaseModel::configuration configuration\endlink property.
     * \sa configuration(), setConfiguration()
     */
    void configurationChanged(Schauer::AbstractConfiguration *configuration);
    /*!
     * \brief Notifier signal for the \link AbstractBaseModel::error error\endlink property.
     * \sa error()
     */
    void errorChanged(int error);

protected:
    AbstractBaseModel(AbstractBaseModelPrivate &dd, QObject *parent = nullptr);

    const std::unique_ptr<AbstractBaseModelPrivate> s_ptr;

    /*!
     * \brief Clears the model data.
     *
     * The default implementation dos nothing.
     */
    virtual void clear();

private:
    Q_DECLARE_PRIVATE_D(s_ptr, AbstractBaseModel)
    Q_DISABLE_COPY(AbstractBaseModel)
};

}

#endif // SCHAUER_ABSTRACTBASEMODEL_H
