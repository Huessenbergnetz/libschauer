/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "sjob_p.h"
#include "logging.h"
#include <QEventLoop>
#include <QTimer>

using namespace Schauer;

SJobPrivate::SJobPrivate()
{

}

SJobPrivate::~SJobPrivate()
{

}

void SJobPrivate::speedTimeout()
{
    Q_Q(SJob);
    Q_EMIT q->speed(q, 0);
    speedTimer->stop();
}

SJob::SJob(QObject *parent)
    : QObject(parent), d_ptr(new SJobPrivate)
{
     d_ptr->q_ptr = this;
}

SJob::SJob(SJobPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

SJob::~SJob()
{
    if (!d_ptr->isFinished) {
        d_ptr->isFinished = true;
        Q_EMIT finished(this, QPrivateSignal());
    }

    delete d_ptr->speedTimer;
}

SJob::Capabilities SJob::capabilities() const
{
    return d_func()->capabilities;
}

bool SJob::isSuspended() const
{
    return d_func()->suspended;
}

void SJob::finishJob(bool emitResult)
{
    Q_D(SJob);
    Q_ASSERT(!d->isFinished);
    d->isFinished = true;

    if (d->eventLoop) {
        d->eventLoop->quit();
    }

    // If we are displaying a progress dialog, remove it first.
    Q_EMIT finished(this, QPrivateSignal());

    if (emitResult) {
        Q_EMIT result(this, QPrivateSignal());
    }

    if (isAutoDelete()) {
        deleteLater();
    }
}

bool SJob::kill(KillVerbosity verbosity)
{
    Q_D(SJob);
    if (d->isFinished) {
        return true;
    }

    if (doKill()) {
        // A subclass can (but should not) call emitResult() or kill()
        // from doKill() and thus set isFinished to true.
        if (!d->isFinished) {
            setError(KilledJobError);
            finishJob(verbosity != Quietly);
        }
        return true;
    } else {
        return false;
    }
}

bool SJob::suspend()
{
    Q_D(SJob);
    if (!d->suspended) {
        if (doSuspend()) {
            d->suspended = true;
            Q_EMIT suspended(this, QPrivateSignal());
            return true;
        }
    }

    return false;
}

bool SJob::resume()
{
    Q_D(SJob);
    if (d->suspended) {
        if (doResume()) {
            d->suspended = false;
            Q_EMIT resumed(this, QPrivateSignal());

            return true;
        }
    }

    return false;
}

bool SJob::doKill()
{
    return false;
}

bool SJob::doSuspend()
{
    return false;
}

bool SJob::doResume()
{
    return false;
}

void SJob::setCapabilities(SJob::Capabilities capabilities)
{
    Q_D(SJob);
    d->capabilities = capabilities;
}

bool SJob::exec()
{
    Q_D(SJob);
    // Usually this job would delete itself, via deleteLater() just after
    // emitting result() (unless configured otherwise). Since we use an event
    // loop below, that event loop will process the deletion event and we'll
    // have been deleted when exec() returns. This crashes, so temporarily
    // suspend autodeletion and manually do it afterwards.
    const bool wasAutoDelete = isAutoDelete();
    setAutoDelete(false);

    Q_ASSERT(!d->eventLoop);

    QEventLoop loop(this);
    d->eventLoop = &loop;

    start();
    if (!d->isFinished) {
        d->eventLoop->exec(QEventLoop::ExcludeUserInputEvents);
    }
    d->eventLoop = nullptr;

    if (wasAutoDelete) {
        deleteLater();
    }
    return (d->error == NoError);
}

int SJob::error() const
{
    return d_func()->error;
}

QString SJob::errorText() const
{
    return d_func()->errorText;
}

QString SJob::errorString() const
{
    return d_func()->errorText;
}

qulonglong SJob::processedAmount(Unit unit) const
{
    if (unit >= UnitsCount) {
        qCWarning(schCore) << "SJob::processedAmount() was called on an invalid Unit" << unit;
        return 0;
    }
    return d_func()->m_jobAmounts[unit].processedAmount;
}

qulonglong SJob::totalAmount(Unit unit) const
{
    if (unit >= UnitsCount) {
        qCWarning(schCore) << "SJob::totalAmount() was called on an invalid Unit" << unit;
        return 0;
    }
    return d_func()->m_jobAmounts[unit].totalAmount;
}

unsigned long SJob::percent() const
{
    return d_func()->percentage;
}

bool SJob::isFinished() const
{
    return d_func()->isFinished;
}

void SJob::setError(int errorCode)
{
    Q_D(SJob);
    d->error = errorCode;
}

void SJob::setErrorText(const QString &errorText)
{
    Q_D(SJob);
    d->errorText = errorText;
}

void SJob::setProcessedAmount(Unit unit, qulonglong amount)
{
    Q_D(SJob);
    auto &[processed, total] = d->m_jobAmounts[unit];

    const bool should_emit = (processed != amount);

    processed = amount;

    if (should_emit) {
        Q_EMIT processedAmount(this, unit, amount);
        Q_EMIT processedAmountChanged(this, unit, amount, QPrivateSignal());
        if (unit == d->progressUnit) {
            Q_EMIT processedSize(this, amount);
            emitPercent(processed, total);
        }
    }
}

void SJob::setTotalAmount(Unit unit, qulonglong amount)
{
    Q_D(SJob);
    auto &[processed, total] = d->m_jobAmounts[unit];

    const bool should_emit = (total != amount);

    total = amount;

    if (should_emit) {
        Q_EMIT totalAmount(this, unit, amount);
        Q_EMIT totalAmountChanged(this, unit, amount, QPrivateSignal());
        if (unit == d->progressUnit) {
            Q_EMIT totalSize(this, amount);
            emitPercent(processed, total);
        }
    }
}

void SJob::setProgressUnit(Unit unit)
{
    Q_D(SJob);
    d->progressUnit = unit;
}

void SJob::setPercent(unsigned long percentage)
{
    Q_D(SJob);
    if (d->percentage != percentage) {
        d->percentage = percentage;
        Q_EMIT percent(this, percentage);
        Q_EMIT percentChanged(this, percentage, QPrivateSignal());
    }
}

void SJob::emitResult()
{
    if (!d_func()->isFinished) {
        finishJob(true);
    }
}

void SJob::emitPercent(qulonglong processedAmount, qulonglong totalAmount)
{
    Q_D(SJob);
    // calculate percents
    if (totalAmount) {
        unsigned long oldPercentage = d->percentage;
        d->percentage = 100.0 * processedAmount / totalAmount;
        if (d->percentage != oldPercentage) {
            Q_EMIT percent(this, d->percentage);
        }
    }
}

void SJob::emitSpeed(unsigned long value)
{
    Q_D(SJob);
    if (!d->speedTimer) {
        d->speedTimer = new QTimer(this);
        connect(d->speedTimer, SIGNAL(timeout()), SLOT(_k_speedTimeout()));
        connect(d->speedTimer, &QTimer::timeout, this, [d]() {
            d->speedTimeout();
        });
    }

    Q_EMIT speed(this, value);
    d->speedTimer->start(5000);   // 5 seconds interval should be enough
}


bool SJob::isAutoDelete() const
{
    Q_D(const SJob);
    return d->isAutoDelete;
}

void SJob::setAutoDelete(bool autodelete)
{
    Q_D(SJob);
    d->isAutoDelete = autodelete;
}

void SJob::setFinishedNotificationHidden(bool hide)
{
    Q_D(SJob);
    d->m_hideFinishedNotification = hide;
}

bool SJob::isFinishedNotificationHidden() const
{
    Q_D(const SJob);
    return d->m_hideFinishedNotification;
}

#include "moc_sjob.cpp"
