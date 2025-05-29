#ifndef MEDIA_H
#define MEDIA_H

#include "external/json.hpp"

namespace media
{
    struct Media
    {
        int         _rating;
        std::string _name;
        std::string _state;
        std::string _type;

        Media(const nlohmann::json json)
        {
            _rating = json["rating"];
            _name   = json["name"];
            _state  = json["state"];
            _type   = json["type"];
        }

        inline const std::string attributesToSql() const
        {
            return "NAME, RATING, STATE, TYPE";
        }

        inline const std::string valuesToSql() const
        {
            std::ostringstream oss;
            oss << "'" << _name << "', " << _rating << ", "
                << "'" << _state << "', "
                << "'" << _type << "'";
            return oss.str();
        }
    };
} // namespace media

#endif
