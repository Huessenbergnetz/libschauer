/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_SJOB_P_H
#define SCHAUER_SJOB_P_H

#include "sjob.h"
#include <QMap>
#include <QEventLoopLocker>

class QTimer;
class QEventLoop;

namespace Schauer {

class SJobPrivate
{
public:
    SJobPrivate();
    virtual ~SJobPrivate();

    SJob *q_ptr = nullptr;

    QString errorText;
    int error = SJob::NoError;
    SJob::Unit progressUnit = SJob::Bytes;
    QMap<SJob::Unit, qulonglong> processedAmount;
    QMap<SJob::Unit, qulonglong> totalAmount;
    unsigned long percentage = 0;
    QTimer *speedTimer = nullptr;
    QEventLoop *eventLoop = nullptr;
    // eventLoopLocker prevents QCoreApplication from exiting when the last
    // window is closed until the job has finished running
    QEventLoopLocker eventLoopLocker;
    SJob::Capabilities capabilities = SJob::NoCapabilities;
    bool suspended = false;
    bool isAutoDelete = true;

    void _k_speedTimeout();

    bool isFinished = false;

    Q_DECLARE_PUBLIC(SJob)
};

}

#endif // SCHAUER_SJOB_P_H
