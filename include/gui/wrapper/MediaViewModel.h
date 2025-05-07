#ifndef MEDIAVIEWMODEL_H
#define MEDIAVIEWMODEL_H

#include <QAbstractTableModel>

#include "globals.h"

struct QMedia
{
    int _id;

    int _rating;
    QString _name;
    QString _state;
    QString _type;

    QMedia(const nlohmann::json &json)
    {
        _id = json["id"];

        _rating = json["rating"];
        _name = QString::fromStdString(json["name"]);
        _state = QString::fromStdString(json["state"]);
        _type = QString::fromStdString(json["type"]);
    }
};

/*
 * This was mainly reused from an older project of myself.
 */
class MediaViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MediaViewModel(QObject *parent = nullptr);

    void setDatalist(const QList<QMedia> &list);

    const QMedia &mediaAt(int row) const;

    const QList<QMedia> &getMediaList() const;

    /**
     * Remove a item from the view model.

     * @return The `id` of the removed media.
     */
    int removeRow(int row, const QModelIndex &parent = QModelIndex());

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QList<QMedia> _data;
};

#endif
