#include "gui/wrapper/MediaViewModel.h"


MediaViewModel::MediaViewModel(QObject *parent)
    : QAbstractTableModel()
{
    QList<QMedia> list;
    setDatalist(list);
}

void MediaViewModel::setDatalist(const QList<QMedia> &list)
{
    beginResetModel();
    _data = list;
    endResetModel();
}

const QMedia &MediaViewModel::mediaAt(int row) const
{
    return _data.at(row);
}

const QList<QMedia> &MediaViewModel::getMediaList() const
{
    return _data;
}

int MediaViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _data.size();
}

int MediaViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4; // amount of all attributes (withoud id) of QMedia
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
