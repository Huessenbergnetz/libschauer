/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "global.h"
#include "logging.h"
#include <QGlobalStatic>
#include <QWriteLocker>
#include <QReadWriteLock>
#include <QTranslator>
#include <QCoreApplication>

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(schCore, "schauer.core")
#else
Q_LOGGING_CATEGORY(schCore, "schauer.core", QtInfoMsg)
#endif

using namespace Schauer;

class DefaultValues
{
public:
    mutable QReadWriteLock lock;

    AbstractConfiguration *configuration() const
    {
        return m_configuration;
    }

    void setConfiguration(AbstractConfiguration *config)
    {
        m_configuration = config;
    }

    AbstractNamFactory *namFactory() const
    {
        return m_namFactory;
    }

    void setNamFactory(AbstractNamFactory *factory)
    {
        m_namFactory = factory;
    }

private:
    AbstractConfiguration *m_configuration = nullptr;
    AbstractNamFactory *m_namFactory = nullptr;
};
Q_GLOBAL_STATIC(DefaultValues, defVals)

AbstractConfiguration *Schauer::defaultConfiguration()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractConfiguration *config = defs->configuration();
    defs->lock.unlock();

    return config;
}

void Schauer::setDefaultConfiguration(AbstractConfiguration *configuration)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(schCore) << "Setting defaultConfiguration to" << configuration;
    defs->setConfiguration(configuration);
}

AbstractNamFactory *Schauer::networkAccessManagerFactory()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNamFactory *namf = defs->namFactory();
    defs->lock.unlock();

    return namf;
}

void Schauer::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(schCore) << "Setting networkAccessManagerFactory to" << factory;
    defs->setNamFactory(factory);
}

bool Schauer::loadTranslations(const QLocale &locale)
{
    auto t = new QTranslator(QCoreApplication::instance());
    if (t->load(locale, QStringLiteral("libschauer"), QStringLiteral("_"), QStringLiteral(SCHAUER_I18NDIR))) {
        if (QCoreApplication::installTranslator(t)) {
            return true;
        } else {
            qCWarning(schCore) << "Failed to install translator for libschauer translations for locale" << locale;
            return false;
        }
    } else {
        qCWarning(schCore) << "Failed to load translations for libschauer for locale" << locale;
        return false;
    }
}
