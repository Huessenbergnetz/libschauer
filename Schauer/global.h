/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_GLOBAL_H
#define SCHAUER_GLOBAL_H

#include "schauer_exports.h"
#include <QLocale>
#include <QVersionNumber>

/*!
 * \brief The root namespace for libschauer.
 */
namespace Schauer {

class AbstractConfiguration;
class AbstractNamFactory;

/*!
 * \brief Sets a pointer to a global default \a configuration.
 * \sa Schauer::defaultConfiguration()
 */
SCHAUER_LIBRARY void setDefaultConfiguration(AbstractConfiguration *configuration);

/*!
 * \brief Returns a pointer to a global default configuration.
 * \sa Schauer::setDefaultConfiguration()
 */
SCHAUER_LIBRARY AbstractConfiguration* defaultConfiguration();

/*!
 * \brief Sets a pointer to a global network access manager \a factory.
 * \sa Schauer::networkAccessManagerFactory()
 */
SCHAUER_LIBRARY void setNetworkAccessManagerFactory(AbstractNamFactory *factory);

/*!
 * \brief Returns a pointer to a global network access manager factory.
 * \sa Schauer::setNetworkAccessManagerFactory()
 */
SCHAUER_LIBRARY AbstractNamFactory* networkAccessManagerFactory();

/*!
 * \brief Load and install the translations for libschauer.
 *
 * Returns \c true on success, otherwise \c false.
 */
SCHAUER_LIBRARY bool loadTranslations(const QLocale &locale = QLocale());

/*!
 * \brief Returns the version of the currently used libschauer.
 */
SCHAUER_LIBRARY QVersionNumber version();

}

#endif // SCHAUER_GLOBAL_H
