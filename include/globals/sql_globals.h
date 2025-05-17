#ifndef SQLGLOBALS_H
#define SQLGLOBALS_H

#include <string>

#include "buildenv.h"

// macros for SQL queries
#define TABLE_NAME          "MEDIA"
#define TABLE_ALL_COL       "ID", "NAME", "RATING", "STATE", "TYPE"

#define DB_NAME             "mtrack.db"

inline const std::string DB_PATH_STR = std::string(APPDATA_DIR_PATH) 
    + "/" + std::string(DB_NAME);

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

#endif
