#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>

#include <sqlite3.h>
#include "json.hpp"

#include "globals.h"

void initDatabase();

void checkTable();

/**
 * Open the database connection.
 */
void openDatabase();

/**
 * Close the database connection.
 */
void closeDatabase();

/**
 * Create a media table inside the database. Should only be called when
 * there is no table.
 */
void createDatabaseTable();

[[
    deprecated
    (
        "Replaced by selectAllJsonQuery but "   \
        "optionparser still use this till further replacement"
    )
]]
void selectAllQuery(std::vector<std::vector<std::string>> &result);

[[
    deprecated
    (
        "Replaced by selectJsonQuery but " \
        "it's still used internally till further replacement."
    )
]]
void selectSpecialQuery(std::vector<std::vector<std::string>> &result,
    const std::string &statement);

nlohmann::json selectAllJsonQuery();

nlohmann::json selectJsonQuery(const std::string &statement);

void addMedia(const media::Media &new_media);

void rmMedia(int rm_id);

#endif
