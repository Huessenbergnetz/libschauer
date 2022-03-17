/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_TESTCONFIG_H
#define SCHAUER_TESTCONFIG_H

#include <Schauer/AbstractConfiguration>

class TestConfig : public Schauer::AbstractConfiguration
{
    Q_OBJECT
public:
    explicit TestConfig(QObject *parent = nullptr);
    ~TestConfig() override;

    void setHost(const QString &host);
    QString host() const override;

private:
    Q_DISABLE_COPY(TestConfig)

    QString m_host = QStringLiteral("localhost");
};

#endif // SCHAUER_TESTCONFIG_H
