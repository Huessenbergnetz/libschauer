/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_VERSIONLISTMODEL_H
#define SCHAUER_VERSIONLISTMODEL_H

#include "schauer_global.h"
#include "abstractversionmodel.h"

namespace Schauer {

class VersionListModelPrivate;

/*!
 * \ingroup data-models
 * \brief List model providing version information.
 * \headerfile "" <Schauer/VersionListModel>
 */
class SCHAUER_LIBRARY VersionListModel : public AbstractVersionModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %VersionListModel with the given \a parent.
     */
    explicit VersionListModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %VersionListModel.
     */
    ~VersionListModel() override;

    /*!
     * \brief Roles used to access the model data.
     *
     * Mainly used to access the model data from QML.
     */
    enum Roles : int {
        NameRole = Qt::UserRole + 1,    /**< The name of the component. Accessor: name */
        VersionRole,                    /**< The version of the component. Accessor: version */
        ApiVersionRole,                 /**< The default (and highest) API version supported by the component. Accessor: apiVersion */
        ArchRole,                       /**< The architectur the component is running on. Accessor: arch */
        BuildTimeRole,                  /**< The date and time the component was compiled. Accessor: buildTime */
        ExperimentalRole,               /**< Indicates if the component is started with experimental features enapled. Accessor: experimental */
        GitCommitRole,                  /**< The Git commit of the source code that was used to build the component. Accessor: gitCommit */
        GoVersionRole,                  /**< The version of Go used to compile the component, and the version of the Go runtime in use. Accessor: goVersion */
        KernelVersionRole,              /**< The kernel version the component is running on. Accessor: kernelVersion */
        MinAPIVersionRole,              /**< The minimum API version that is supported by this component. Accessor: minApiVersion */
        OsRole                          /**< The operating system the component is running on. Accessor: os */
    };

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    Q_DECLARE_PRIVATE_D(s_ptr, VersionListModel)
    Q_DISABLE_COPY(VersionListModel)
};

}

#endif // SCHAUER_VERSIONLISTMODEL_H
