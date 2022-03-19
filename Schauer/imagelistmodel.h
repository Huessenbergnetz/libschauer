/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_IMAGESLISTMODEL_H
#define SCHAUER_IMAGESLISTMODEL_H

#include "schauer_global.h"
#include "abstractimagesmodel.h"

namespace Schauer {

class ImageListModelPrivate;

/*!
 * \ingroup dta-models
 * \brief List model providing available images.
 * \headerfile "" <Schauer/ImageListModel>
 */
class SCHAUER_LIBRARY ImageListModel : public AbstractImagesModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %ImageListModel with the given \a parent.
     */
    explicit ImageListModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %ImageListModel.
     */
    ~ImageListModel() override;

    enum Roles : int {
        IdRole = Qt::UserRole + 1,
        ParentIdRole,
        RepoTagsRole,
        RepoDigestsRole,
        CreatedRole,
        SizeRole,
        VirtualSizeRole,
        SharedSizeRole,
        LabelsRole,
        ContainersRole
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
    Q_DECLARE_PRIVATE_D(s_ptr, ImageListModel)
    Q_DISABLE_COPY(ImageListModel)
};

}

#endif // SCHAUER_IMAGESLISTMODEL_H
