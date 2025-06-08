#ifndef MEDIAVIEWMODEL_H
#define MEDIAVIEWMODEL_H

#include <QAbstractTableModel>

#include "gui/QMedia.h"

/**
 * This was mainly reused from an older project of myself.
 *
 * @note Because this model is designed for MVC pattern in Qt style
 * the model will update the media data / interact with the db
 * itself.
 */
class MediaViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MediaViewModel(const QList<QMedia> &list, QObject *parent = nullptr);

    const QMedia &getMediaAt(int row) const;

    bool insertRow(const QMedia &new_media);

    void removeRow(int row);

    bool editRow(int row, const QMedia &edited_media);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant
    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(
        int section, Qt::Orientation orientation, int role = Qt::DisplayRole
    ) const override;

private:
    QList<QMedia> _data;
};

MediaViewModel *createLatestMediaViewModel();

#endif
