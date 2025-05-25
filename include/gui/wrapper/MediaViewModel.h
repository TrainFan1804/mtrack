#ifndef MEDIAVIEWMODEL_H
#define MEDIAVIEWMODEL_H

#include <QAbstractTableModel>

#include "gui/QMedia.h"

/*
 * This was mainly reused from an older project of myself.
 */
class MediaViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MediaViewModel(const QList <QMedia> &list, QObject *parent = nullptr);

    const QMedia &getMediaAt(int row) const;

    bool insertRow(const QMedia &new_media);

    /**
     * Remove a row from the view model.
     * 
     * @return The id of the removed media. When non media was removed this method
     * will return -1.
     */
    int removeRow(int row);

    bool editRow(int row, const QMedia &edited_media);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QList<QMedia> _data;
};

MediaViewModel *createLatestMediaViewModel();

#endif
