/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testconfig.h"

TestConfig::TestConfig(QObject *parent)
    : Schauer::AbstractConfiguration(parent)
{

}

TestConfig::~TestConfig() = default;

QString TestConfig::host() const
{
    return m_host;
}

void TestConfig::setHost(const QString &host)
{
    m_host = host;
}

#include "moc_testconfig.cpp"
