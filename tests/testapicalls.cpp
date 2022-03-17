/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QSignalSpy>
#include <QJsonObject>
#include <QJsonArray>
#include "testconfig.h"
#include <Schauer/GetVersionJob>
#include <Schauer/VersionListModel>
#include <Schauer/ListImagesJob>
#include <Schauer/VersionListModel>
#include <Schauer/ImagesListModel>

using namespace Schauer;

class ApiCallsTest : public QObject
{
    Q_OBJECT
public:
    explicit ApiCallsTest(QObject *parent = nullptr);
    ~ApiCallsTest() override;

private slots:
    void initTestCase();

    void testUseDefaultConfig();
    void testUseCustomConfig();
    void testGetVersionJobSync();
    void testGetVersionJobAsync();
    void testVersionListModelSync();
    void testVersionListModelAsync();
    void testListImagesJobSync();
    void testListImagesJobAsync();
    void testImagesListModelSync();
    void testImagesListModelAsync();

    void cleanupTestCase();

private:
    TestConfig *m_config = nullptr;
};

ApiCallsTest::ApiCallsTest(QObject *parent)
    : QObject(parent)
{

}

ApiCallsTest::~ApiCallsTest() = default;

void ApiCallsTest::initTestCase()
{
    m_config = new TestConfig(this);
    Schauer::setDefaultConfiguration(m_config);
}

void ApiCallsTest::testUseDefaultConfig()
{
    auto job = new GetVersionJob(this);
    QCOMPARE(job->configuration(), nullptr);
    QSignalSpy confChangedSpy(job, &Job::configurationChanged);
    QVERIFY(job->exec());
    QCOMPARE(confChangedSpy.count(), 1);
    QCOMPARE(confChangedSpy.at(0).at(0).value<TestConfig*>(), m_config);
}

void ApiCallsTest::testUseCustomConfig()
{
    auto job = new GetVersionJob(this);
    job->setAutoDelete(false);
    auto conf = new TestConfig(this);
    job->setConfiguration(conf);
    QSignalSpy confChangedSpy(job, &Job::configurationChanged);
    QVERIFY(job->exec());
    QVERIFY(confChangedSpy.empty());
    QCOMPARE(job->configuration(), conf);
}

void ApiCallsTest::testGetVersionJobSync()
{
    auto job = new GetVersionJob(this);
    QVERIFY(job->exec());
    QVERIFY(!job->replyData().object().empty());
}

void ApiCallsTest::testGetVersionJobAsync()
{
    auto job = new GetVersionJob(this);
    QSignalSpy finishedSpy(job, &SJob::finished);
    QSignalSpy resultSpy(job, &SJob::result);
    QSignalSpy succeededSpy(job, &Job::succeeded);
    job->start();
    QVERIFY(finishedSpy.wait());
    QCOMPARE(finishedSpy.count(), 1);
    QCOMPARE(finishedSpy.at(0).at(0).value<GetVersionJob*>(), job);
    QCOMPARE(resultSpy.count(), 1);
    QCOMPARE(resultSpy.at(0).at(0).value<GetVersionJob*>(), job);
    QCOMPARE(succeededSpy.count(), 1);
    QVERIFY(!succeededSpy.at(0).at(0).toJsonDocument().object().empty());
}

void ApiCallsTest::testVersionListModelSync()
{
    auto model = new VersionListModel(this);
    QVERIFY(model->load(AbstractBaseModel::LoadSync));
    QVERIFY(model->rowCount() > 0);
    QVERIFY(!model->version().isEmpty());
    QVERIFY(!model->apiVersion().isEmpty());
    QVERIFY(!model->minApiVersion().isEmpty());
    QVERIFY(!model->goVersion().isEmpty());
    QVERIFY(!model->os().isEmpty());
    QVERIFY(!model->arch().isEmpty());
    QVERIFY(!model->kernelVersion().isEmpty());
    QVERIFY(model->buildTime().isValid());
}

void ApiCallsTest::testVersionListModelAsync()
{
    auto model = new VersionListModel(this);
    QSignalSpy loadedSpy(model, &AbstractBaseModel::loaded);
    QSignalSpy isLoadingSpy(model, &AbstractBaseModel::isLoadingChanged);
    model->load();
    QVERIFY(loadedSpy.wait());
    QCOMPARE(loadedSpy.count(), 1);
    QCOMPARE(isLoadingSpy.count(), 2);
    QCOMPARE(isLoadingSpy.at(0).at(0).toBool(), true);
    QCOMPARE(isLoadingSpy.at(1).at(0).toBool(), false);
    QVERIFY(model->rowCount() > 0);
    QVERIFY(!model->version().isEmpty());
    QVERIFY(!model->apiVersion().isEmpty());
    QVERIFY(!model->minApiVersion().isEmpty());
    QVERIFY(!model->goVersion().isEmpty());
    QVERIFY(!model->os().isEmpty());
    QVERIFY(!model->arch().isEmpty());
    QVERIFY(!model->kernelVersion().isEmpty());
    QVERIFY(model->buildTime().isValid());
}

void ApiCallsTest::testListImagesJobSync()
{
    auto job = new ListImagesJob(this);
    QVERIFY(job->exec());
    QVERIFY(!job->replyData().array().empty());
}

void ApiCallsTest::testListImagesJobAsync()
{
    auto job = new ListImagesJob(this);
    QSignalSpy finishedSpy(job, &SJob::finished);
    QSignalSpy resultSpy(job, &SJob::result);
    QSignalSpy succeededSpy(job, &Job::succeeded);
    job->start();
    QVERIFY(finishedSpy.wait());
    QCOMPARE(finishedSpy.count(), 1);
    QCOMPARE(finishedSpy.at(0).at(0).value<ListImagesJob*>(), job);
    QCOMPARE(resultSpy.count(), 1);
    QCOMPARE(resultSpy.at(0).at(0).value<ListImagesJob*>(), job);
    QCOMPARE(succeededSpy.count(), 1);
    QVERIFY(!succeededSpy.at(0).at(0).toJsonDocument().array().empty());
}

void ApiCallsTest::testImagesListModelSync()
{
    auto model = new ImagesListModel(this);
    QVERIFY(model->load(AbstractBaseModel::LoadSync));
    QVERIFY(model->rowCount() > 0);
    QVERIFY(model->containsRepoTag(QLatin1String("hello-world")));
    QVERIFY(model->containsRepoTag(QLatin1String("hello-world"), QLatin1String("latest")));
    QVERIFY(!model->containsRepoTag(QLatin1String("dummer-schiss")));
    QVERIFY(!model->containsRepoTag(QLatin1String("dummer-schiss"), QLatin1String("latest")));
}

void ApiCallsTest::testImagesListModelAsync()
{
    auto model = new ImagesListModel(this);
    QSignalSpy loadedSpy(model, &AbstractBaseModel::loaded);
    QSignalSpy isLoadingSpy(model, &AbstractBaseModel::isLoadingChanged);
    model->load();
    QVERIFY(loadedSpy.wait());
    QCOMPARE(loadedSpy.count(), 1);
    QCOMPARE(isLoadingSpy.count(), 2);
    QCOMPARE(isLoadingSpy.at(0).at(0).toBool(), true);
    QCOMPARE(isLoadingSpy.at(1).at(0).toBool(), false);
    QVERIFY(model->rowCount() > 0);
}

void ApiCallsTest::cleanupTestCase()
{

}

QTEST_MAIN(ApiCallsTest)

#include "testapicalls.moc"
