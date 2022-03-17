/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractconfiguration.h"

using namespace Schauer;

AbstractConfiguration::AbstractConfiguration(QObject *parent) : QObject(parent)
{

}

AbstractConfiguration::~AbstractConfiguration() = default;

QString AbstractConfiguration::username() const
{
    return QString();
}

QString AbstractConfiguration::password() const
{
    return QString();
}

QString AbstractConfiguration::host() const
{
    return QStringLiteral("localhost");
}

int AbstractConfiguration::port() const
{
    return 2375;
}

bool AbstractConfiguration::useSsl() const
{
    return false;
}

bool AbstractConfiguration::ignoreSslErrors() const
{
    return false;
}

#include "moc_abstractconfiguration.cpp"
