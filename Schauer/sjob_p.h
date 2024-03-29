/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_SJOB_P_H
#define SCHAUER_SJOB_P_H

#include "sjob.h"
#include <QMap>
#include <QEventLoopLocker>
#include <array>

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

    struct Amounts {
        qulonglong processedAmount = 0;
        qulonglong totalAmount = 0;
    };

    std::array<Amounts, SJob::UnitsCount> m_jobAmounts;

    unsigned long percentage = 0;
    QTimer *speedTimer = nullptr;
    QEventLoop *eventLoop = nullptr;
    // eventLoopLocker prevents QCoreApplication from exiting when the last
    // window is closed until the job has finished running
    QEventLoopLocker eventLoopLocker;
    SJob::Capabilities capabilities = SJob::NoCapabilities;
    bool suspended = false;
    bool isAutoDelete = true;
    bool m_hideFinishedNotification = false;

    void speedTimeout();

    bool isFinished = false;

    Q_DECLARE_PUBLIC(SJob)
};

}

#endif // SCHAUER_SJOB_P_H
