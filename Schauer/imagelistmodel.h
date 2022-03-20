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
 * \ingroup data-models
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
        IdRole = Qt::UserRole + 1,  /**< The ID of the image. Accessor: id, Type: QString */
        ParentIdRole,               /**< The ID of an optional parent image. Accessor: parentId, Type: QString */
        RepoTagsRole,               /**< Tags used to identify the image. Accessor: repoTags, Type: QStringList */
        RepoDigestsRole,            /**< Digests used to identify the image. Accessor: repoDigests, Type: QStringList */
        CreatedRole,                /**< Date and time the image has been created. Accessor: created, Type: QDateTime */
        SizeRole,                   /**< The size of the image. Accessor: size, Type: qint64 */
        VirtualSizeRole,            /**< The virtual size of the image. Accessor: virtualSize, Type: qint64 */
        SharedSizeRole,             /**< The shared size of the image. Accessor: sharedSize, Type: qint64 */
        LabelsRole,                 /**< Used defind key/value pairs. Accessor: labels, Type: QMap<QString, QString> */
        ContainersRole              /**< Number of containers created from this image. Accessor: containers, Type: int */
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
