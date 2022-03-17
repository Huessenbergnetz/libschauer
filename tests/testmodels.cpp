/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QSignalSpy>
#include <QAbstractItemModelTester>
#include <Schauer/VersionListModel>
#include <Schauer/ImagesListModel>

using namespace Schauer;

class ModelTest : public QObject
{
    Q_OBJECT
public:
    ModelTest(QObject *parent = nullptr) : QObject(parent) {}

    ~ModelTest() override {}

private Q_SLOTS:
    void initTestCase() {}

    void testVersionListModel();
    void testImagesListModel();

    void cleanupTestCase() {}
};

void ModelTest::testVersionListModel()
{
    auto versionListModel = new VersionListModel(this);
    new QAbstractItemModelTester(versionListModel, this);
}

void ModelTest::testImagesListModel()
{
    auto model = new ImagesListModel(this);
    new QAbstractItemModelTester(model, this);

    // test showAll property
    QSignalSpy showAllSpy(model, &AbstractImagesModel::showAllChanged);
    QVERIFY(!model->showAll());
    model->setShowAll(true);
    QCOMPARE(showAllSpy.count(), 1);
    const QVariantList showAllSpyArgs = showAllSpy.takeFirst();
    QCOMPARE(showAllSpyArgs.at(0).toBool(), true);
    QVERIFY(model->showAll());

    // test showDigests property
    QSignalSpy showDigestsSpy(model, &AbstractImagesModel::showDigestsChanged);
    QVERIFY(!model->showDigests());
    model->setShowDigests(true);
    QCOMPARE(showDigestsSpy.count(), 1);
    const QVariantList showDigestsSpyArgs = showDigestsSpy.takeFirst();
    QCOMPARE(showDigestsSpyArgs.at(0).toBool(), true);
    QVERIFY(model->showDigests());
}

QTEST_MAIN(ModelTest)

#include "testmodels.moc"
