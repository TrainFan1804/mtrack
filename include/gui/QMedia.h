#ifndef QMEDIA_H
#define QMEDIA_H

#include <QString>
#include <QMetaType>

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

Q_DECLARE_METATYPE(QMedia)

/**
 * This is absoluty st*pid that there are multiple structures to describe media...
 */
inline media::Media qMediaToMedia(const QMedia &q_media)
{
    nlohmann::json json;
    json["rating"] = q_media._rating;
    json["name"] = q_media._name.toStdString();
    json["state"] = q_media._state.toStdString();
    json["type"] = q_media._type.toStdString();
    media::Media media(json);
    return media;
}

#endif
