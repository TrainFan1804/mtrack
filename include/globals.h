#ifndef GLOBALS_H 
#define GLOBALS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <cctype>

#include "buildenv.h"

#ifndef VERSION
#define VERSION "unknown"
#endif

#define DB_NAME "mtrack.db"
#define LOG_NAME "debug.log"

inline const std::string DB_PATH_STR = std::string(APPDATA_DIR_PATH) 
    + "/" + std::string(DB_NAME);
inline const std::string LOG_PATH_STR = std::string(LOG_DIR_PATH)
    + "/" +  std::string(LOG_NAME);

inline bool log_active = false;

namespace media
{
    struct Media
    {
        int _media_id;
        int _rating;
        std::string _name;
        std::string _state;
        std::string _type;

        Media(const std::string &name, int rating,
            const std::string &state, const std::string &type)
        {
            _rating = rating;
            _name = name;
            _state = state;
            _type = type;
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
