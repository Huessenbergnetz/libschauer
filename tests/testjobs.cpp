/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QSignalSpy>
#include <Schauer/ListImagesJob>
#include <Schauer/ListContainersJob>
#include "testconfig.h"

using namespace Schauer;

class JobsTest : public QObject
{
    Q_OBJECT
public:
    JobsTest(QObject *parent = nullptr) : QObject(parent) {}

    ~JobsTest() override {}

private Q_SLOTS:
    void initTestCase() {}

    void testSetConfiguration();
    void testMissingConfiguration();
    void testMissingHost();
    void testListImagesJob();
    void testListContainersJob();

    void cleanupTestCase() {}
};

void JobsTest::testSetConfiguration()
{
    auto job = new ListImagesJob(this);
    QCOMPARE(job->configuration(), nullptr);
    auto conf = new TestConfig(this);
    QSignalSpy confChangedSpy(job, &Job::configurationChanged);
    job->setConfiguration(conf);
    QCOMPARE(job->configuration(), conf);
    QCOMPARE(confChangedSpy.count(), 1);
    QCOMPARE(confChangedSpy.at(0).at(0).value<TestConfig*>(), conf);
}

void JobsTest::testMissingConfiguration()
{
    auto job = new ListImagesJob(this);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::MissingConfig));
}

void JobsTest::testMissingHost()
{
    auto job = new ListImagesJob(this);
    auto conf = new TestConfig(this);
    conf->setHost(QString());
    job->setConfiguration(conf);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::MissingHost));
}

void JobsTest::testListImagesJob()
{
    auto job = new ListImagesJob(this);

    // test showAll property
    QSignalSpy showAllSpy(job, &ListImagesJob::showAllChanged);
    QCOMPARE(job->showAll(), false); // default value
    job->setShowAll(true);
    QCOMPARE(showAllSpy.count(), 1);
    const QVariantList showAllSpyArgs = showAllSpy.takeFirst();
    QCOMPARE(showAllSpyArgs.at(0).toBool(), true);
    QCOMPARE(job->showAll(), true);

    // test showDigests property
    QSignalSpy showDigestsSpy(job, &ListImagesJob::showDigestsChanged);
    QCOMPARE(job->showDigests(), false); // defautl value
    job->setShowDigests(true);
    QCOMPARE(showDigestsSpy.count(), 1);
    const QVariantList showDigestsSpyArgs = showDigestsSpy.takeFirst();
    QCOMPARE(showDigestsSpyArgs.at(0).toBool(), true);
    QCOMPARE(job->showDigests(), true);
}

void JobsTest::testListContainersJob()
{
    auto job = new ListContainersJob(this);

    // test limit property
    const int newLimit = 25;
    QSignalSpy limitChangedSpy(job, &ListContainersJob::limitChanged);
    QCOMPARE(job->limit(), 0); // default value
    job->setLimit(newLimit);
    QCOMPARE(limitChangedSpy.count(), 1);
    const QVariantList limitChangedSpyArgs = limitChangedSpy.takeFirst();
    QCOMPARE(limitChangedSpyArgs.at(0).toInt(), newLimit);
    QCOMPARE(job->limit(), newLimit);

    // test showAll property
    QSignalSpy showAllChangedSpy(job, &ListContainersJob::showAllChanged);
    QCOMPARE(job->showAll(), false); // default value
    job->setShowAll(true);
    QCOMPARE(showAllChangedSpy.count(), 1);
    const QVariantList showAllChangedArgs = showAllChangedSpy.takeFirst();
    QCOMPARE(showAllChangedArgs.at(0).toBool(), true);
    QCOMPARE(job->showAll(), true);

    // test showSizeproperty
    QSignalSpy showSizeSpy(job, &ListContainersJob::showSizeChanged);
    QCOMPARE(job->showSize(), false); // default value
    job->setShowSize(true);
    QCOMPARE(showSizeSpy.count(), 1);
    const QVariantList showSizeArgs = showSizeSpy.takeFirst();
    QCOMPARE(showSizeArgs.at(0).toBool(), true);
    QCOMPARE(job->showSize(), true);
}

QTEST_MAIN(JobsTest)

#include "testjobs.moc"
