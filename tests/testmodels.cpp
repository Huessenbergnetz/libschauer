/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QSignalSpy>
#include <QAbstractItemModelTester>
#include <Schauer/VersionListModel>
#include <Schauer/ImageListModel>
#include <Schauer/ContainerListModel>

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
    void testImageListModel();
    void testContainerListModel();

    void cleanupTestCase() {}
};

void ModelTest::testVersionListModel()
{
    auto versionListModel = new VersionListModel(this);
    new QAbstractItemModelTester(versionListModel, this);
}

void ModelTest::testImageListModel()
{
    auto model = new ImageListModel(this);
    new QAbstractItemModelTester(model, this);

    // test showAll property
    QSignalSpy showAllSpy(model, &AbstractImageModel::showAllChanged);
    QVERIFY(!model->showAll());
    model->setShowAll(true);
    QCOMPARE(showAllSpy.count(), 1);
    const QVariantList showAllSpyArgs = showAllSpy.takeFirst();
    QCOMPARE(showAllSpyArgs.at(0).toBool(), true);
    QVERIFY(model->showAll());

    // test showDigests property
    QSignalSpy showDigestsSpy(model, &AbstractImageModel::showDigestsChanged);
    QVERIFY(!model->showDigests());
    model->setShowDigests(true);
    QCOMPARE(showDigestsSpy.count(), 1);
    const QVariantList showDigestsSpyArgs = showDigestsSpy.takeFirst();
    QCOMPARE(showDigestsSpyArgs.at(0).toBool(), true);
    QVERIFY(model->showDigests());
}

void ModelTest::testContainerListModel()
{
    auto model = new ContainerListModel(this);
    new QAbstractItemModelTester(model, this);

    // test showAll property
    {
        QSignalSpy spy(model, &AbstractContainerModel::showAllChanged);
        QCOMPARE(model->showAll(), false); // default value
        model->setShowAll(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(model->showAll(), true);
    }

    // test showSize property
    {
        QSignalSpy spy(model, &AbstractContainerModel::showSizeChanged);
        QCOMPARE(model->showSize(), false); // default value
        model->setShowSize(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(model->showSize(), true);
    }
}

QTEST_MAIN(ModelTest)

#include "testmodels.moc"
