/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_ABSTRACTVERSIONMODEL_P_H
#define SCHAUER_ABSTRACTVERSIONMODEL_P_H

#include "abstractversionmodel.h"
#include "abstractbasemodel_p.h"
#include <vector>

namespace Schauer {

struct VersionComponent {
    QString name;
    QString apiVersion;
    QString arch;
    QString gitCommit;
    QString goVersion;
    QString kernelVersion;
    QString minApiVersion;
    QString os;
    QString version;
    QDateTime buildTime;
    bool experimental = false;

    VersionComponent(const QString &_name, const QString &_apiVersion, const QString &_arch, const QDateTime &_buildTime, const QString &_gitCommit, const QString &_goVersion, const QString &_kernelVersion, const QString &_minApiVersion, const QString &_os, const QString &_version, bool _experimental) :
        name(_name),
        apiVersion(_apiVersion),
        arch(_arch),
        gitCommit(_gitCommit),
        goVersion(_goVersion),
        kernelVersion(_kernelVersion),
        minApiVersion(_minApiVersion),
        os(_os),
        version(_version),
        buildTime(_buildTime),
        experimental(_experimental)
    {}

    ~VersionComponent() = default;

    VersionComponent(const VersionComponent &other) :
        name(other.name),
        apiVersion(other.apiVersion),
        arch(other.arch),
        gitCommit(other.gitCommit),
        goVersion(other.goVersion),
        kernelVersion(other.kernelVersion),
        minApiVersion(other.minApiVersion),
        os(other.os),
        version(other.version),
        buildTime(other.buildTime),
        experimental(other.experimental)
    {}

    VersionComponent(VersionComponent &&other) :
        name(std::move(other.name)),
        apiVersion(std::move(other.apiVersion)),
        arch(std::move(other.arch)),
        gitCommit(std::move(other.gitCommit)),
        goVersion(std::move(other.goVersion)),
        kernelVersion(std::move(other.kernelVersion)),
        minApiVersion(std::move(other.minApiVersion)),
        os(std::move(other.os)),
        version(std::move(other.version)),
        buildTime(std::move(other.buildTime)),
        experimental(std::move(other.experimental))
    {}

    VersionComponent& operator=(const VersionComponent &other) = default;

    VersionComponent& operator=(VersionComponent &&other) = default;
};

class AbstractVersionModelPrivate : public AbstractBaseModelPrivate
{
public:
    AbstractVersionModelPrivate(AbstractVersionModel *q);

    ~AbstractVersionModelPrivate();

    void setupJob() override;
    bool loadFromJson(const QJsonDocument &json) override;

    std::vector<VersionComponent> components;
    QString apiVersion;
    QString arch;
    QDateTime buildTime;
    QString gitCommit;
    QString goVersion;
    QString kernelVersion;
    QString minApiVersion;
    QString os;
    QString platformName;
    QString version;

private:
    Q_DISABLE_COPY(AbstractVersionModelPrivate)
    Q_DECLARE_PUBLIC(AbstractVersionModel)
};

}

#endif // SCHAUER_ABSTRACTVERSIONMODEL_P_H
