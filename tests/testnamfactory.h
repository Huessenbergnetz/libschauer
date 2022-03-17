/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_TESTNAMFACTORY_H
#define SCHAUER_TESTNAMFACTORY_H

#include <Schauer/AbstractNamFactory>

class TestNamFactory : public Schauer::AbstractNamFactory
{
public:
    ~TestNamFactory() override;

    QNetworkAccessManager *create(QObject *parent = nullptr) override;
};

#endif // SCHAUER_TESTNAMFACTORY_H
