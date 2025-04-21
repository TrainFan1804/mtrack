#ifndef GLOBALS_H 
#define GLOBALS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>

#ifndef VERSION
#define VERSION "unknown"
#endif

#define DB_NAME "mtrack.db"
#define LOG_NAME "debug.log"
// Relative path from $HOME to the app data
#define REL_APPDATA_PATH ".local/share/mtrack"
// Relative path from $REL_APPDATA_PATH to the log dir
#define REL_LOG_PATH "log"

inline const std::filesystem::path APPDATA_PATH = std::string(std::getenv("HOME"))
    + "/" + std::string(REL_APPDATA_PATH);
inline const std::filesystem::path LOG_PATH = (APPDATA_PATH / REL_LOG_PATH);

inline const std::string DB_PATH_STR = (APPDATA_PATH / DB_NAME).string();
inline const std::string LOG_PATH_STR = (LOG_PATH / LOG_NAME).string();

inline bool log_active = false;

namespace media
{
    struct Media
    {
        int _media_id;
        int _rating;
        std::string _name;

        Media()
        {
            _rating = 1;
            _name = "";
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

#endif
