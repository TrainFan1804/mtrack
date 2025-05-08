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
    MediaViewModel(QObject *parent, const QList <QMedia> &list);

    const QMedia &getMediaAt(int row) const;

    bool insertRow(const QMedia &new_media);

    int removeRow(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QList<QMedia> _data;
};

#endif
