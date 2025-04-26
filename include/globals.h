#ifndef GLOBALS_H 
#define GLOBALS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <cctype>

#include "json.hpp"

#include "buildenv.h"

#ifndef VERSION
#define VERSION "unknown"
#endif

// response codes for the communication between frontend and backend
#define ASK_DATA            "100"
#define ADD_RESPONSE        "101"
#define RM_RESPONSE         "102"
#define SEND_RESPONSE       "200"
#define TRN_END             "300"
#define SEND_ID             "301"
#define WRG_FORMAT          "400"
#define DB_ERROR            "500"

#define RESPONSE_CODE_SIZE  4   // prefix = 3, . = 1

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
    };
}

inline std::string getCustomCurrentTimestamp() 
{
    auto time_now = std::time(nullptr);
    auto tm = *std::localtime(&time_now);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    return oss.str();
}

/*
    Why isn't there a function like this is std?
*/
inline std::string strToLower(const std::string str)
{
    std::string res = str;
    std::transform(
        res.begin(), res.end(), res.begin(),
        [](unsigned char c){
            return std::tolower(c);
        }
    );
    return res;
}

#endif
