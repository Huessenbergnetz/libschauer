/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testconfig.h"
#include "testnamfactory.h"
#include <Schauer/job.h>

#include <QObject>
#include <QTest>

#include <memory>

class DefaultValuesTest : public QObject
{
    Q_OBJECT
public:
    explicit DefaultValuesTest(QObject *parent = nullptr);
    ~DefaultValuesTest() override = default;

private slots:
    void testDefaultConfiguration();
    void testNetworkAccessManagerFactory();
};

DefaultValuesTest::DefaultValuesTest(QObject *parent)
    : QObject(parent)
{

}

void DefaultValuesTest::testDefaultConfiguration()
{
    auto c = new TestConfig(this);
    Schauer::setDefaultConfiguration(c);
    QCOMPARE(Schauer::defaultConfiguration(), c);
}

void DefaultValuesTest::testNetworkAccessManagerFactory()
{
    auto namf = std::make_unique<TestNamFactory>();
    Schauer::setNetworkAccessManagerFactory(namf.get());
    QCOMPARE(Schauer::networkAccessManagerFactory(), namf.get());
}

QTEST_MAIN(DefaultValuesTest)

#include "testdefaultvalues.moc"
