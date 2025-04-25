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

void selectAllQuery(std::vector<std::vector<std::string>> &result);

void selectSpecialQuery(std::vector<std::vector<std::string>> &result,
    const std::string &statement);

nlohmann::json selectAllJsonQuery();

void addMedia(const media::Media &new_media);

void rmMedia(int rm_id);

#endif
