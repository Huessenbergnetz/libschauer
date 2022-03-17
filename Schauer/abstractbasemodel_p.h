/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTBASEMODEL_P_H
#define SCHAUER_ABSTRACTBASEMODEL_P_H

#include "abstractbasemodel.h"
#include "job.h"

class QJsonDocument;

namespace Schauer {

class AbstractBaseModelPrivate
{
public:
    AbstractBaseModelPrivate(AbstractBaseModel *q);

    virtual ~AbstractBaseModelPrivate();

    AbstractConfiguration *configuration = nullptr;
    Job *job = nullptr;

    virtual void setupJob();
    bool startJob(AbstractBaseModel::LoadMode mode);
    virtual bool loadFromJson(const QJsonDocument &json);
    void finishLoading(int error, const QString &errorString = QString());

    void setIsLoading(bool isLoading);
    bool isLoading() const;

    void setError(int code, const QString &errorString = QString());
    int error() const;
    QString errorString() const;

protected:
    AbstractBaseModel *q_ptr = nullptr;

private:
    QString m_errorString;
    int m_error = 0;
    bool m_isLoading = false;

    Q_DISABLE_COPY(AbstractBaseModelPrivate)
    Q_DECLARE_PUBLIC(AbstractBaseModel)
};

}

#endif // SCHAUER_ABSTRACTBASEMODEL_P_H
