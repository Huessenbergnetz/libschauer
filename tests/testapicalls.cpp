/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QSignalSpy>
#include <QJsonObject>
#include <QJsonArray>
#include "testconfig.h"
#include <Schauer/Global>
#include <Schauer/GetVersionJob>
#include <Schauer/VersionListModel>
#include <Schauer/ListImagesJob>
#include <Schauer/VersionListModel>
#include <Schauer/ImageListModel>
#include <Schauer/CreateContainerJob>
#include <Schauer/StartContainerJob>
#include <Schauer/StopContainerJob>
#include <Schauer/RemoveContainerJob>
#include <Schauer/ContainerListModel>
#include <Schauer/CreateExecInstanceJob>
#include <Schauer/StartExecInstanceJob>

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
    void testImageListModelSync();
    void testImageListModelAsync();
    void testContainerOperationsSync();
    void testContainerOperationsAsync();

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

void ApiCallsTest::testImageListModelSync()
{
    auto model = new ImageListModel(this);
    QVERIFY(model->load(AbstractBaseModel::LoadSync));
    QVERIFY(model->rowCount() > 0);
    QVERIFY(model->containsRepoTag(QLatin1String("nginx")));
    QVERIFY(model->containsRepoTag(QLatin1String("nginx"), QLatin1String("latest")));
    QVERIFY(!model->containsRepoTag(QLatin1String("dummer-schiss")));
    QVERIFY(!model->containsRepoTag(QLatin1String("dummer-schiss"), QLatin1String("latest")));
}

void ApiCallsTest::testImageListModelAsync()
{
    auto model = new ImageListModel(this);
    QSignalSpy loadedSpy(model, &AbstractBaseModel::loaded);
    QSignalSpy isLoadingSpy(model, &AbstractBaseModel::isLoadingChanged);
    model->load();
    QVERIFY(loadedSpy.wait());
    QCOMPARE(loadedSpy.count(), 1);
    QCOMPARE(isLoadingSpy.count(), 2);
    QCOMPARE(isLoadingSpy.at(0).at(0).toBool(), true);
    QCOMPARE(isLoadingSpy.at(1).at(0).toBool(), false);
    QVERIFY(model->rowCount() > 0);
    QVERIFY(model->containsRepoTag(QLatin1String("nginx")));
    QVERIFY(model->containsRepoTag(QLatin1String("nginx"), QLatin1String("latest")));
    QVERIFY(!model->containsRepoTag(QLatin1String("dummer-schiss")));
    QVERIFY(!model->containsRepoTag(QLatin1String("dummer-schiss"), QLatin1String("latest")));
}

void ApiCallsTest::testContainerOperationsSync()
{
    const QString containerName = QStringLiteral("/my-little-container-sync");
    QVariantHash containerConfig;
    containerConfig.insert(QStringLiteral("Image"), QStringLiteral("nginx:latest"));

    // create a new container
    auto create = new CreateContainerJob(this);
    create->setName(containerName);
    create->setContainerConfig(containerConfig);
    QVERIFY(create->exec());
    const QString containerId = create->replyData().object().value(QStringLiteral("Id")).toString();
    QVERIFY(!containerId.isEmpty());

    auto model = new ContainerListModel(this);
    model->setShowAll(true);
    model->load(AbstractBaseModel::LoadSync);
    QVERIFY(model->rowCount() > 0);
    QVERIFY(model->contains(containerName));
    QVERIFY(model->contains(containerId));
    QVERIFY(!model->contains(QLatin1String("/ahsdfashdf")));
    QVERIFY(!model->contains(QLatin1String("adasdf")));
    QVERIFY(!model->contains(QLatin1String()));
    QVERIFY(model->containsImage(QLatin1String("nginx:latest")));
    QVERIFY(!model->containsImage(QLatin1String("dummer:schiss")));
    QVERIFY(!model->containsImage(QLatin1String()));

    // start container
    auto start = new StartContainerJob(this);
    start->setId(containerId);
    QVERIFY(start->exec());

    // execute command inside container
    auto execCreate = new CreateExecInstanceJob(this);
    execCreate->setId(containerId);
    execCreate->setCmd(QStringList({QStringLiteral("ls"), QStringLiteral("-l")}));
    QVERIFY(execCreate->exec());

    const QString execId = execCreate->replyData().object().value(QLatin1String("Id")).toString();

    auto execStart = new StartExecInstanceJob(this);
    execStart->setId(execId);
    execStart->setDetach(true);
    QVERIFY(execStart->exec());

    // stop container
    auto stop = new StopContainerJob(this);
    stop->setId(containerId);
    QVERIFY(stop->exec());

    // remove the container
    auto remove = new RemoveContainerJob(this);
    remove->setId(containerId);
    QVERIFY(remove->exec());
}

void ApiCallsTest::testContainerOperationsAsync()
{
    const QString containerName = QStringLiteral("/my-little-container-async");
    QVariantHash containerConfig;
    containerConfig.insert(QStringLiteral("Image"), QStringLiteral("nginx:latest"));

    QString containerId;

    // create a new container
    auto create = new CreateContainerJob(this);
    create->setName(containerName);
    create->setContainerConfig(containerConfig);
    {
        QSignalSpy finishedSpy(create, &SJob::finished);
        QSignalSpy resultSpy(create, &SJob::result);
        QSignalSpy succeededSpy(create, &Job::succeeded);
        QSignalSpy failedSpy(create, &Job::failed);
        create->start();
        QVERIFY(finishedSpy.wait());
        QCOMPARE(create->error(), 0);
        QCOMPARE(finishedSpy.count(), 1);
        QCOMPARE(finishedSpy.at(0).at(0).value<CreateContainerJob*>(), create);
        QCOMPARE(resultSpy.count(), 1);
        QCOMPARE(resultSpy.at(0).at(0).value<CreateContainerJob*>(), create);
        QCOMPARE(failedSpy.count(), 0);
        QCOMPARE(succeededSpy.count(), 1);
        containerId = succeededSpy.at(0).at(0).toJsonDocument().object().value(QStringLiteral("Id")).toString();
        QVERIFY(!containerId.isEmpty());
    }

    auto model = new ContainerListModel(this);
    model->setShowAll(true);
    {
        QSignalSpy loadedSpy(model, &AbstractBaseModel::loaded);
        QSignalSpy isLoadingSpy(model, &AbstractBaseModel::isLoadingChanged);
        QSignalSpy errorSpy(model, &AbstractBaseModel::errorChanged);
        model->load();
        QVERIFY(loadedSpy.wait());
        QCOMPARE(loadedSpy.count(), 1);
        QCOMPARE(isLoadingSpy.count(), 2);
        QCOMPARE(isLoadingSpy.at(0).at(0).toBool(), true);
        QCOMPARE(isLoadingSpy.at(1).at(0).toBool(), false);
        QCOMPARE(errorSpy.count(), 0);
        QVERIFY(model->rowCount() > 0);
        QCOMPARE(model->error(), 0);
        QVERIFY(model->contains(containerName));
        QVERIFY(model->contains(containerId));
        QVERIFY(!model->contains(QLatin1String("/ahsdfashdf")));
        QVERIFY(!model->contains(QLatin1String("adasdf")));
        QVERIFY(!model->contains(QLatin1String()));
        QVERIFY(model->containsImage(QLatin1String("nginx:latest")));
        QVERIFY(!model->containsImage(QLatin1String("dummer:schiss")));
        QVERIFY(!model->containsImage(QLatin1String()));
    }

    // start container
    auto start = new StartContainerJob(this);
    start->setId(containerId);
    {
        QSignalSpy finishedSpy(start, &SJob::finished);
        QSignalSpy resultSpy(start, &SJob::result);
        QSignalSpy succeededSpy(start, &Job::succeeded);
        QSignalSpy failedSpy(start, &Job::failed);
        start->start();
        QVERIFY(finishedSpy.wait());
        QCOMPARE(start->error(), 0);
        QCOMPARE(finishedSpy.count(), 1);
        QCOMPARE(finishedSpy.at(0).at(0).value<StartContainerJob*>(), start);
        QCOMPARE(resultSpy.count(), 1);
        QCOMPARE(resultSpy.at(0).at(0).value<StartContainerJob*>(), start);
        QCOMPARE(failedSpy.count(), 0);
        QCOMPARE(succeededSpy.count(), 1);
        QVERIFY(succeededSpy.at(0).at(0).toJsonDocument().isEmpty());
    }

    // run command inside container
    auto execCreate = new CreateExecInstanceJob(this);
    execCreate->setId(containerId);
    execCreate->setCmd(QStringList({QStringLiteral("ls"), QStringLiteral("-l")}));
    {
        QSignalSpy finishedSpy(execCreate, &SJob::finished);
        QSignalSpy resultSpy(execCreate, &SJob::result);
        QSignalSpy succeededSpy(execCreate, &Job::succeeded);
        QSignalSpy failedSpy(execCreate, &Job::failed);
        execCreate->start();
        QVERIFY(finishedSpy.wait());
        QCOMPARE(execCreate->error(), 0);
        QCOMPARE(finishedSpy.count(), 1);
        QCOMPARE(finishedSpy.at(0).at(0).value<CreateExecInstanceJob*>(), execCreate);
        QCOMPARE(resultSpy.count(), 1);
        QCOMPARE(resultSpy.at(0).at(0).value<CreateExecInstanceJob*>(), execCreate);
        QCOMPARE(failedSpy.count(), 0);
        QCOMPARE(succeededSpy.count(), 1);
        QVERIFY(!succeededSpy.at(0).at(0).toJsonDocument().object().value(QStringLiteral("Id")).toString().isEmpty());
    }

    const QString execId = execCreate->replyData().object().value(QStringLiteral("Id")).toString();
    QVERIFY(!execId.isEmpty());

    auto execStart = new StartExecInstanceJob(this);
    execStart->setId(execId);
    execStart->start();
    {
        QSignalSpy finishedSpy(execStart, &SJob::finished);
        QSignalSpy resultSpy(execStart, &SJob::result);
        QSignalSpy succeededSpy(execStart, &Job::succeeded);
        QSignalSpy failedSpy(execStart, &Job::failed);
        execStart->start();
        QVERIFY(finishedSpy.wait());
        QCOMPARE(execStart->error(), 0);
        QCOMPARE(finishedSpy.count(), 1);
        QCOMPARE(finishedSpy.at(0).at(0).value<StartExecInstanceJob*>(), execStart);
        QCOMPARE(resultSpy.count(), 1);
        QCOMPARE(resultSpy.at(0).at(0).value<StartExecInstanceJob*>(), execStart);
        QCOMPARE(failedSpy.count(), 0);
        QCOMPARE(succeededSpy.count(), 1);
        QVERIFY(succeededSpy.at(0).at(0).toJsonDocument().isEmpty());
    }

    // stop container
    auto stop = new StopContainerJob(this);
    stop->setId(containerId);
    {
        QSignalSpy finishedSpy(stop, &SJob::finished);
        QSignalSpy resultSpy(stop, &SJob::result);
        QSignalSpy succeededSpy(stop, &Job::succeeded);
        QSignalSpy failedSpy(stop, &Job::failed);
        stop->start();
        QVERIFY(finishedSpy.wait());
        QCOMPARE(stop->error(), 0);
        QCOMPARE(finishedSpy.count(), 1);
        QCOMPARE(finishedSpy.at(0).at(0).value<StopContainerJob*>(), stop);
        QCOMPARE(resultSpy.count(), 1);
        QCOMPARE(resultSpy.at(0).at(0).value<StopContainerJob*>(), stop);
        QCOMPARE(failedSpy.count(), 0);
        QCOMPARE(succeededSpy.count(), 1);
        QVERIFY(succeededSpy.at(0).at(0).toJsonDocument().isEmpty());
    }

    // remove the container
    auto remove = new RemoveContainerJob(this);
    remove->setId(containerId);
    {
        QSignalSpy finishedSpy(remove, &SJob::finished);
        QSignalSpy resultSpy(remove, &SJob::result);
        QSignalSpy succeededSpy(remove, &Job::succeeded);
        QSignalSpy failedSpy(remove, &Job::failed);
        remove->start();
        QVERIFY(finishedSpy.wait());
        QCOMPARE(remove->error(), 0);
        QCOMPARE(finishedSpy.count(), 1);
        QCOMPARE(finishedSpy.at(0).at(0).value<RemoveContainerJob*>(), remove);
        QCOMPARE(resultSpy.count(), 1);
        QCOMPARE(resultSpy.at(0).at(0).value<RemoveContainerJob*>(), remove);
        QCOMPARE(failedSpy.count(), 0);
        QCOMPARE(succeededSpy.count(), 1);
        QVERIFY(succeededSpy.at(0).at(0).toJsonDocument().isEmpty());
    }
}

void ApiCallsTest::cleanupTestCase()
{

}

QTEST_MAIN(ApiCallsTest)

#include "testapicalls.moc"
