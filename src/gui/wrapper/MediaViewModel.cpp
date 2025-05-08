#include "gui/wrapper/MediaViewModel.h"


MediaViewModel::MediaViewModel(QObject *parent, const QList<QMedia> &list)
    : QAbstractTableModel()
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
   return true;
}

int MediaViewModel::removeRow(int row)
{
    if (row < 0 || row >= _data.size())
        return -1;

    beginRemoveRows(QModelIndex(), row, row);
    auto rm_media = _data.at(row);
    _data.removeAt(row);
    endRemoveRows();
    return rm_media._id;
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
    return 4;
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
            case 0: return media._name;
            case 1: return media._rating;
            case 2: return media._state;
            case 3: return media._type;
        }
    }
    return QVariant();
}

QVariant MediaViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0: return "Name";
            case 1: return "Rating";
            case 2: return "State";
            case 3: return "Type";
        }
    }
    return QVariant();
}
