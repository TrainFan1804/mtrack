#ifndef GLOBALS_H 
#define GLOBALS_H

#include <string>
#include <sstream>
#include <filesystem>
#include <cctype>

#include "external/json.hpp"

#include "buildenv.h"

#ifndef VERSION
#define VERSION "unknown"
#endif

// macros for SQL queries
#define TABLE_NAME          "MEDIA"
#define TABLE_ALL_COL       "ID", "NAME", "RATING", "STATE", "TYPE"

#define DB_NAME "mtrack.db"
#define LOG_NAME "debug.log"

inline const std::string DB_PATH_STR = std::string(APPDATA_DIR_PATH) 
    + "/" + std::string(DB_NAME);
inline const std::string LOG_PATH_STR = std::string(LOG_DIR_PATH)
    + "/" +  std::string(LOG_NAME);

inline bool log_active = false;

inline const std::string SQL_JSON_SELECT_ALL = std::string("SELECT json_group_array(json_object(")
    + "'id', ID, "
    + "'name', NAME, "
    + "'rating', RATING, "
    + "'state', STATE, "
    + "'type', TYPE"
    + ")) AS json_result FROM " + std::string(TABLE_NAME) + ";";

inline const std::string SQL_CREATE = "CREATE TABLE IF NOT EXISTS " 
    + std::string(TABLE_NAME) + "(" 
    + "ID INTEGER PRIMARY KEY, "
    + "NAME TEXT NOT NULL, "
    + "RATING INT NOT NULL, "
    + "STATE TEXT NOT NULL, "
    + "TYPE TEXT NOT NULL"
    + ");";

namespace media
{
    struct Media
    {
        int _rating;
        std::string _name;
        std::string _state;
        std::string _type;

        [[deprecated("After the CLI works with JSON dataformat this will be removed")]]
        Media(const std::string &name, int rating,
            const std::string &state, const std::string &type)
        {
            _rating = rating;
            _name = name;
            _state = state;
            _type = type;
        }

        Media(const nlohmann::json json)
        {
            _rating = json["rating"];
            _name = json["name"];
            _state = json["state"];
            _type = json["type"];
        }

        inline const std::string attributesToSql() const
        {
            return "NAME, RATING, STATE, TYPE";
        }

        inline const std::string valuesToSql() const
        {
            std::ostringstream oss;
            oss << "'" << _name << "', "
                << _rating << ", "
                << "'" << _state << "', "
                << "'" << _type << "'";
            return oss.str();
        }
    };
}

#endif
