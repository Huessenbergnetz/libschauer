/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_CONTAINERLISTMODEL_H
#define SCHAUER_CONTAINERLISTMODEL_H

#include "schauer_global.h"
#include "abstractcontainermodel.h"

namespace Schauer {

class ContainerListModelPrivate;

/*!
 * \ingroup data-models
 * \brief List model providing availabel containers.
 * \headerfile "" <Schauer/ContainerListModel>
 */
class SCHAUER_LIBRARY ContainerListModel : public AbstractContainerModel
{
    Q_OBJECT
public:
    /*!
     * \brief Conctructs a new %ContainerListModel with the given \a parent.
     */
    explicit ContainerListModel(QObject *parent = nullptr);

    /*!
     * \brief Destrys the %ContainerListModel object.
     */
    ~ContainerListModel() override;

    /*!
     * \brief Roles used to access the model data.
     *
     * Mainly used to access the model data from QML.
     */
    enum Roles : int {
        IdRole = Qt::UserRole + 1,  /**< The ID of the container. Accessor: id, Type: QString */
        NamesRole,                  /**< The names that the container has been given. Accessor: names, Type: QStringList */
        ImageRole,                  /**< The name of the image usedn when creating the container: Accessor: image, Type: QString */
        ImageIdRole,                /**< The ID of the image that the container was created from. Accessor: imageId, Type: QString */
        CommandRole,                /**< Command to run when starting the container: Accessor: command, Type: QString */
        CreatedRole,                /**< When the container was created. Accessor: created, Type: QDateTIme */
        StateRole,                  /**< The state of the container (e.g. Exited). Accessor: state, Type: QString */
        StatusRole,                 /**< Additional human readable status of the container (e.g. Exit 0). Accessor: status, Type: QString */
        LabelsRole,                 /**< User defined key/value metadata for for the container. Accessor: labels, Type: QMap<QString,QString> */
        SizeRwRole,                 /**< The size of the files that have been created or changed by the container. Accessor: sizeRw, Type: quint64*/
        SizeRootFsRole              /**< The total size of all the files in the container. Accessor: sizeRootFs, Type: quint64 */
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
    Q_DECLARE_PRIVATE_D(s_ptr, ContainerListModel)
    Q_DISABLE_COPY(ContainerListModel)
};

}

#endif // SCHAUER_CONTAINERLISTMODEL_H
