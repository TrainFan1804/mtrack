#include "gui/wrapper/MediaViewModel.h"

#include "db/database_service.h"

MediaViewModel::MediaViewModel(const QList<QMedia> &list, QObject *parent)
    : QAbstractTableModel(parent)
{
    beginResetModel();
    _data = list;
    endResetModel();
}

const QMedia &MediaViewModel::getMediaAt(int row) const
{
    return _data.at(row);
}

bool MediaViewModel::insertRow(const QMedia &new_media)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _data.append(new_media);
    endInsertRows();
    addMedia(new_media.unwrap());
    return true;
}

void MediaViewModel::removeRow(int row)
{
    if (row < 0 || row >= _data.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    auto rm_media = _data.at(row);
    _data.removeAt(row);
    endRemoveRows();

    int del_id = rm_media.getId();
    // this is just temp to avoid access the database when none is selected
    if (del_id <= -1)
        return;
    rmMedia(del_id);
}

bool MediaViewModel::editRow(int row, const QMedia &edited_media)
{
    if (row < 0 || row >= _data.size())
        return false;

    _data[row]                         = edited_media;
    // get left and right cell of the edited row and send signal to qt to change
    // those data
    QModelIndex edited_row_left_index  = index(row, 0);
    QModelIndex edited_row_right_index = index(row, columnCount() - 1);
    emit        dataChanged(edited_row_left_index, edited_row_right_index);

    editMedia(edited_media.getId(), edited_media.unwrap());
    return true;
}

int MediaViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _data.size();
}

int MediaViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // amount of all attributes (withoud id) of QMedia (because id should
    // not be showed inside the TableModel)
    return attributeAmount();
}

QVariant MediaViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _data.size())
        return QVariant();

    auto media = _data.at(index.row());

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0: return media.name();
        case 1: return media.state();
        case 2: return media.type();
        case 3: return media.rating();
        }
    }
    return QVariant();
}

QVariant MediaViewModel::headerData(
    int section, Qt::Orientation orientation, int role
) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: return "Name";
        case 1: return "State";
        case 2: return "Type";
        case 3: return "Rating";
        }
    }
    return QVariant();
}

MediaViewModel *createLatestMediaViewModel()
{
    // fetch data from database
    auto          data_json = selectAllJsonQuery();
    QList<QMedia> view_data;
    for (auto json_obj : data_json)
    {
        QMedia media(json_obj);
        view_data << media;
    }
    return new MediaViewModel(view_data);
}
