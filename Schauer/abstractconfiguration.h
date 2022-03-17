/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTCONFIGURATION_H
#define SCHAUER_ABSTRACTCONFIGURATION_H

#include "schauer_global.h"
#include <QObject>

namespace Schauer {

/*!
 * \brief Configuration settings base class.
 *
 * Reimplement this class to provide configuration for accessing the Docker
 * daemon REST API.
 *
 * \par See also
 * \li Schauer::setDefaultConfiguration()
 * \li Schauer::defaultConfiguration()
 *
 * \headerfile "" <Schauer/AbstractConfiguration>
 */
class SCHAUER_LIBRARY AbstractConfiguration : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %AbstractConfiguration object with the given \a parent.
     */
    explicit AbstractConfiguration(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractConfiguration object.
     */
    ~AbstractConfiguration() override;

    /*!
     * \brief Returns the username needed for authentication.
     * The default implementation returns an empty string.
     */
    virtual QString username() const;

    /*!
     * \brief Returns the password needed for authentication.
     * The default implementation returns an empty string.
     */
    virtual QString password() const;

    /*!
     * \brief Returns the host to connect to.
     * The default implementation returns "localhost".
     */
    virtual QString host() const;

    /*!
     * \brief Returns the port the Docker daemon is listening on.
     * The default implementation returns \c 2375.
     */
    virtual int port() const;

    /*!
     * \brief Returns \c true if the connection should use SSL/TLS, otherwise returns \c false.
     * The default implementation returns \c false.
     */
    virtual bool useSsl() const;

    /*!
     * \brief Returns \c true if SSL/TLS errors should be ignored, otherwise returns \c false.
     * The default implementation return \c false.
     */
    virtual bool ignoreSslErrors() const;

private:
    Q_DISABLE_COPY(AbstractConfiguration)
};

}

#endif // SCHAUER_ABSTRACTCONFIGURATION_H
