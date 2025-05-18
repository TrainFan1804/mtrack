#ifndef QMEDIA_H
#define QMEDIA_H

#include <QString>
#include <QMetaType>

#include "Media.h"

/**
 * media::Media doesn't save the id it's associated with in the DB because those 
 * where originally designed for CLI only and CLI doens't need information about
 * the media id.
 * 
 * But there is the need for internal id's for media to save and remove them from
 * the DB correctly when using the GUI. It's also a wrapper for QString to let
 * them display easily in the Qt GUI.
 */
class QMedia
{
public:
    QMedia(const nlohmann::json &json)
        : _media(json)
    {
        _id = json["id"];
    }

    int getId() const { return _id; }
    
    void setId(int id) { _id = id; }

    int rating() const { return _media._rating; }

    QString name() const { return QString::fromStdString(_media._name); }

    QString state() const { return QString::fromStdString(_media._state); }

    QString type() const { return QString::fromStdString(_media._type); }

    /**
     * This should only be used when you want to extract the wrapped media
     * to work with the database API, not to change the internal state of
     * QMedia directly.
     */
    media::Media unwrap() { return _media; }

private:
    int _id;
    media::Media _media;
};

Q_DECLARE_METATYPE(QMedia)

inline int attributeAmount()
{
    return 4;
}

#endif
