/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractbasemodel_p.h"
#include "logging.h"
#include <QJsonDocument>
#include <QObject>

using namespace Schauer;

/*!
 * \internal
 * \class AbstractBaseModelPrivate
 * \brief Internal private base class for all data models.
 */

AbstractBaseModelPrivate::AbstractBaseModelPrivate(AbstractBaseModel *q)
    : q_ptr(q)
{

}

AbstractBaseModelPrivate::~AbstractBaseModelPrivate() = default;

bool AbstractBaseModelPrivate::isLoading() const
{
    return m_isLoading;
}

void AbstractBaseModelPrivate::setupJob()
{

}

bool AbstractBaseModelPrivate::startJob(AbstractBaseModel::LoadMode mode)
{
    setIsLoading(true);
    setError(0, QString());

    Q_Q(AbstractBaseModel);

    q->clear();

    job->setConfiguration(q->configuration());

    if (mode == AbstractBaseModel::LoadAsync) {
        QObject::connect(job, &Job::result, q, [this](SJob *sjob){
            if (sjob->error()) {
                finishLoading(sjob->error(), sjob->errorString());
            } else {
                Job *_job = qobject_cast<Job* >(sjob);
                loadFromJson(_job->replyData());
            }
        });
        job->start();
        return true;
    } else {
        if (job->exec()) {
            return loadFromJson(job->replyData());
        } else {
            finishLoading(job->error(), job->errorString());
            return false;
        }
    }
}

bool AbstractBaseModelPrivate::loadFromJson(const QJsonDocument &json)
{
    Q_UNUSED(json);
    finishLoading(0);
    return true;
}

void AbstractBaseModelPrivate::finishLoading(int error, const QString &errorString)
{
    setError(error, errorString);
    setIsLoading(false);
    Q_Q(AbstractBaseModel);
    Q_EMIT q->loaded();
}

void AbstractBaseModelPrivate::setIsLoading(bool isLoading)
{
    Q_Q(AbstractBaseModel);
    if (m_isLoading != isLoading) {
        m_isLoading = isLoading;
        Q_EMIT q->isLoadingChanged(m_isLoading);
    }
}

int AbstractBaseModelPrivate::error() const
{
    return m_error;
}

QString AbstractBaseModelPrivate::errorString() const
{
    return m_errorString;
}

void AbstractBaseModelPrivate::setError(int code, const QString &errorString)
{
    Q_Q(AbstractBaseModel);
    if (m_error != code || m_errorString != errorString) {
        m_error = code;
        m_errorString = errorString;
        Q_EMIT q->errorChanged(m_error);
    }
}

QStringList AbstractBaseModelPrivate::jsonArrayToStringList(const QJsonArray &array)
{
    QStringList _list;
    if (!array.empty()) {
        _list.reserve(array.size());
        for (const QJsonValue &val : array) {
            _list << val.toString();
        }
    }
    return _list;
}

QStringList AbstractBaseModelPrivate::jsonArrayToStringList(const QJsonValue &value)
{
    return AbstractBaseModelPrivate::jsonArrayToStringList(value.toArray());
}

QMap<QString,QString> AbstractBaseModelPrivate::jsonObjectToStringMap(const QJsonObject &object)
{
    QMap<QString,QString> _map;
    if (!object.isEmpty()) {
        const QStringList keys = object.keys();
        for (const QString &key : keys) {
            _map.insert(key, object.value(key).toString());
        }
    }
    return _map;
}

QMap<QString,QString> AbstractBaseModelPrivate::jsonObjectToStringMap(const QJsonValue &value)
{
    return AbstractBaseModelPrivate::jsonObjectToStringMap(value.toObject());
}

AbstractBaseModel::AbstractBaseModel(QObject *parent)
    : QAbstractItemModel(parent), s_ptr(new AbstractBaseModelPrivate(this))
{

}

AbstractBaseModel::AbstractBaseModel(AbstractBaseModelPrivate &dd, QObject *parent)
    : QAbstractItemModel(parent), s_ptr(&dd)
{

}

AbstractBaseModel::~AbstractBaseModel() = default;

AbstractConfiguration* AbstractBaseModel::configuration() const
{
    Q_D(const AbstractBaseModel);
    return d->configuration;
}

void AbstractBaseModel::setConfiguration(AbstractConfiguration *configuration)
{
    Q_D(AbstractBaseModel);
    if (configuration != d->configuration) {
        qCDebug(schCore) << "Changing configuration from" << d->configuration << "to" << configuration;
        d->configuration = configuration;
        Q_EMIT configurationChanged(d->configuration);
    }
}

bool AbstractBaseModel::isLoading() const
{
    Q_D(const AbstractBaseModel);
    return d->isLoading();
}

int AbstractBaseModel::error() const
{
    Q_D(const AbstractBaseModel);
    return d->error();
}

QString AbstractBaseModel::errorString() const
{
    Q_D(const AbstractBaseModel);
    return d->errorString();
}

bool AbstractBaseModel::load(Schauer::AbstractBaseModel::LoadMode mode)
{
    Q_D(AbstractBaseModel);
    d->setupJob();
    return d->startJob(mode);
}

void AbstractBaseModel::clear()
{

}

#include "moc_abstractbasemodel.cpp"
