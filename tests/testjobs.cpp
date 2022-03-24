/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QSignalSpy>
#include <Schauer/ListImagesJob>
#include <Schauer/ListContainersJob>
#include <Schauer/CreateContainerJob>
#include <Schauer/StartContainerJob>
#include <Schauer/StopContainerJob>
#include <Schauer/RemoveContainerJob>
#include <Schauer/CreateExecInstanceJob>
#include <Schauer/StartExecInstanceJob>
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
    void testCreateContainerJob();
    void testStartContainerJob();
    void testStopContainerJob();
    void testRemoveContainerJob();
    void testCreateExecInstanceJob();
    void testStartExecInstanceJob();

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
    job->setAutoDelete(false);

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
    job->setAutoDelete(false);

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

void JobsTest::testCreateContainerJob()
{
    auto job = new CreateContainerJob(this);
    job->setConfiguration(new TestConfig(this));
    job->setAutoDelete(false);

    // check missing image name because of empty container config
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    // test name property
    const QString newContainerName = QStringLiteral("/my-container");
    QSignalSpy nameChangedSpy(job, &CreateContainerJob::nameChanged);
    QVERIFY(job->name().isEmpty()); // default value
    job->setName(newContainerName);
    QCOMPARE(nameChangedSpy.count(), 1);
    QCOMPARE(nameChangedSpy.at(0).at(0).toString(), newContainerName);
    QCOMPARE(job->name(), newContainerName);

    // test containerConfig property
    const QVariantHash newContainerConfig({{QStringLiteral("Image"), QStringLiteral("hello-world")}});
    QSignalSpy ccSpy(job, &CreateContainerJob::containerConfigChanged);
    QVERIFY(job->containerConfig().empty()); // default value
    job->setContainerConfig(newContainerConfig);
    QCOMPARE(ccSpy.count(), 1);
    QCOMPARE(ccSpy.at(0).at(0).toHash(), newContainerConfig);
    QCOMPARE(job->containerConfig(), newContainerConfig);

    // check missing image name
    const QVariantHash missingImageKey({{QStringLiteral("Tty"), true}});
    job->setContainerConfig(missingImageKey);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    const QVariantHash missingImageValue({{QStringLiteral("Image"), QString()}});
    job->setContainerConfig(missingImageValue);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    // check invalid container name
    const QString invalidContainerName = QStringLiteral("//_kacke");
    job->setName(invalidContainerName);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));
}

void JobsTest::testStartContainerJob()
{
    auto job = new StartContainerJob(this);
    job->setConfiguration(new TestConfig(this));
    job->setAutoDelete(false);

    // test missing id
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    //test id property
    const QString newId = QStringLiteral("8dfafdbc3a40");
    QSignalSpy idChangedSpy(job, &StartContainerJob::idChanged);
    QVERIFY(job->id().isEmpty()); // default value
    job->setId(newId);
    QCOMPARE(idChangedSpy.count(), 1);
    QCOMPARE(idChangedSpy.at(0).at(0).toString(), newId);
    QCOMPARE(job->id(), newId);

    // test detachKeys property
    const QString newDetachKeys = QStringLiteral("ctrl-[");
    QSignalSpy detachKeysChangedSpy(job, &StartContainerJob::detachKeysChanged);
    QVERIFY(job->detachKeys().isEmpty()); // default value
    job->setDetachKeys(newDetachKeys);
    QCOMPARE(detachKeysChangedSpy.count(), 1);
    QCOMPARE(detachKeysChangedSpy.at(0).at(0).toString(), newDetachKeys);
    QCOMPARE(job->detachKeys(), newDetachKeys);

    // test invalid detachKeys
    job->setDetachKeys(QStringLiteral("abc"));
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    job->setDetachKeys(QStringLiteral("!"));
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    job->setDetachKeys(QStringLiteral("ctrl-!"));
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));
}

void JobsTest::testStopContainerJob()
{
    auto job = new StopContainerJob(this);
    job->setConfiguration(new TestConfig(this));
    job->setAutoDelete(false);

    // test missing id
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    // test id property
    const QString newId = QStringLiteral("8dfafdbc3a40");
    QSignalSpy idChangedSpy(job, &StopContainerJob::idChanged);
    QVERIFY(job->id().isEmpty()); // default value
    job->setId(newId);
    QCOMPARE(idChangedSpy.count(), 1);
    QCOMPARE(idChangedSpy.at(0).at(0).toString(), newId);
    QCOMPARE(job->id(), newId);

    // test timeout property
    const int newTimeout = 300;
    QSignalSpy timeoutChangedSpy(job, &StopContainerJob::timeoutChanged);
    QCOMPARE(job->timeout(), 0); // default value
    job->setTimeout(newTimeout);
    QCOMPARE(timeoutChangedSpy.count(), 1);
    QCOMPARE(timeoutChangedSpy.at(0).at(0).toInt(), newTimeout);
    QCOMPARE(job->timeout(), newTimeout);
}

void JobsTest::testRemoveContainerJob()
{
    auto job = new RemoveContainerJob(this);
    job->setConfiguration(new TestConfig(this));
    job->setAutoDelete(false);

    // test missing id
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    // test id property
    {
        const QString id = QStringLiteral("8dfafdbc3a40");
        QSignalSpy spy(job, &RemoveContainerJob::idChanged);
        QVERIFY(job->id().isEmpty()); // default value
        job->setId(id);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), id);
        QCOMPARE(job->id(), id);
    }

    // test removeAnonVolumes property
    {
        QSignalSpy spy(job, &RemoveContainerJob::removeAnonVolumesChanged);
        QVERIFY(!job->removeAnonVolumes()); // default value
        job->setRemoveAnonVolumes(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->removeAnonVolumes(), true);
    }

    // test force property
    {
        QSignalSpy spy(job, &RemoveContainerJob::forceChanged);
        QVERIFY(!job->force()); // default value
        job->setForce(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->force(), true);
    }

    // test removeLinks property
    {
        QSignalSpy spy(job, &RemoveContainerJob::removeLinksChanged);
        QVERIFY(!job->removeLinks()); // default value
        job->setRemoveLinks(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->removeLinks(), true);
    }
}

void JobsTest::testCreateExecInstanceJob()
{
    auto job = new CreateExecInstanceJob(this);
    job->setConfiguration(new TestConfig(this));
    job->setAutoDelete(false);

    // test missing id
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    // test id property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::idChanged);
        QVERIFY(job->id().isEmpty()); // default value
        job->setId(QStringLiteral("new-id"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("new-id"));
        QCOMPARE(job->id(), QStringLiteral("new-id"));
    }

    // test attachStdin property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::attachStdinChanged);
        QVERIFY(!job->attachStdin()); // default value
        job->setAttachStdin(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->attachStdin(), true);
    }

    // test attachStdout property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::attachStdoutChanged);
        QVERIFY(!job->attachStdout()); // default value
        job->setAttachStdout(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->attachStdout(), true);
    }

    // test attachStderr property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::attachStderrChanged);
        QVERIFY(!job->attachStderr()); // default value
        job->setAttachStderr(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->attachStderr(), true);
    }

    // test detachKeys property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::detachKeysChanged);
        QVERIFY(job->detachKeys().isEmpty()); // default value
        job->setDetachKeys(QStringLiteral("z"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("z"));
        QCOMPARE(job->detachKeys(), QStringLiteral("z"));
    }

    // test invalid detachKeys
    {
        job->setDetachKeys(QStringLiteral("abc"));
        QVERIFY(!job->exec());
        QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));
    }

    // test tty property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::ttyChanged);
        QVERIFY(!job->tty()); // default value
        job->setTty(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->tty(), true);
    }

    // test env property
    {
        const QStringList newEnvList({QStringLiteral("FOO=bar"), QStringLiteral("BAR=baz")});
        QSignalSpy spy(job, &CreateExecInstanceJob::envChanged);
        QVERIFY(job->env().empty()); // default value;
        job->setEnv(newEnvList);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toStringList(), newEnvList);
        QCOMPARE(job->env(), newEnvList);

        job->addEnv(QStringLiteral("FORENAME=Tommy"));
        QVERIFY(job->env().contains(QStringLiteral("FORENAME=Tommy")));
        QCOMPARE(spy.count(), 2);

        job->addEnv(QStringLiteral("SURNAME"), QStringLiteral("Atkins"));
        QVERIFY(job->env().contains(QStringLiteral("SURNAME=Atkins")));
        QCOMPARE(spy.count(), 3);

        job->removeEnv(QStringLiteral("BAR=baz"));
        QVERIFY(!job->env().contains(QStringLiteral("BAR=baz")));
        QCOMPARE(spy.count(), 4);
    }

    // test cmd property missing
    {
        QVERIFY(!job->exec());
        QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));
    }

    // test cmd property
    {
        const QStringList newCmd({QStringLiteral("ls"), QStringLiteral("-l")});
        QSignalSpy spy(job, &CreateExecInstanceJob::cmdChanged);
        QVERIFY(job->cmd().empty()); // default value
        job->setCmd(newCmd);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toStringList(), newCmd);
        QCOMPARE(job->cmd(), newCmd);
    }

    // test privileged property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::privilegedChanged);
        QVERIFY(!job->privileged()); // default value
        job->setPrivileged(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->privileged(), true);
    }

    // test user property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::userChanged);
        QVERIFY(job->user().isEmpty()); // default value
        job->setUser(QStringLiteral("my-user"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("my-user"));
        QCOMPARE(job->user(), QStringLiteral("my-user"));
    }

    // test workingDir property
    {
        QSignalSpy spy(job, &CreateExecInstanceJob::workingDirChanged);
        QVERIFY(job->workingDir().isEmpty()); // default value
        job->setWorkingDir(QStringLiteral("/my/working/dir"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("/my/working/dir"));
        QCOMPARE(job->workingDir(), QStringLiteral("/my/working/dir"));
    }
}

void JobsTest::testStartExecInstanceJob()
{
    auto job = new StartExecInstanceJob(this);
    job->setConfiguration(new TestConfig(this));
    job->setAutoDelete(false);

    // test missing id
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Schauer::InvalidInput));

    // test id property
    {
        QSignalSpy spy(job, &StartExecInstanceJob::idChanged);
        QVERIFY(job->id().isEmpty()); // default value
        job->setId(QStringLiteral("new-id"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("new-id"));
        QCOMPARE(job->id(), QStringLiteral("new-id"));
    }

    // test detach property
    {
        QSignalSpy spy(job, &StartExecInstanceJob::detachChanged);
        QVERIFY(job->detach()); // default value
        job->setDetach(false);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), false);
        QCOMPARE(job->detach(), false);
    }

    // test tty property
    {
        QSignalSpy spy(job, &StartExecInstanceJob::ttyChanged);
        QVERIFY(!job->tty()); // default value
        job->setTty(true);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(job->tty(), true);
    }
}

QTEST_MAIN(JobsTest)

#include "testjobs.moc"
