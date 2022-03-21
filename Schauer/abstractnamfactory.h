/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTNAMFACTORY_H
#define SCHAUER_ABSTRACTNAMFACTORY_H

#include "schauer_exports.h"

class QObject;
class QNetworkAccessManager;

namespace Schauer {

/*!
 * \brief Creates QNetworkAccessManager instances for Schauer API classes.
 *
 * Schauer API classes use QNetworkAccessManager for all network related operations. By implementing
 * a factory, it is possible to provide custom QNetworkAccessManager instances with specialized caching,
 * proxy and cookie support to the API classes.
 *
 * To im plement a factory, subclass AbstractNamFactory and implement the virtual create() method,
 * then assign it to the Schauer API classes using Schauer::setNetworkAccessManagerFactory().
 *
 * \headerfile "" <Schauer/AbstractNamFactory>
 */
class SCHAUER_LIBRARY AbstractNamFactory
{
public:
    /*!
     * \brief Destroys the factory. The default implementation does nothing.
     */
    virtual ~AbstractNamFactory();

    /*!
     * \brief Creates and returns a network access manager with the specified \a parent.
     * This method must return a new QNetworkAccessManager instance each time it is called.
     */
    virtual QNetworkAccessManager *create(QObject *parent) = 0;
};

}

#endif // SCHAUER_ABSTRACTNAMFACTORY_H
