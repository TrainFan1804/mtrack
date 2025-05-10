#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>

#include <sqlite3.h>
#include "external/json.hpp"

#include "globals.h"

void initDatabase();

void checkTable();

/**
 * Open the database connection.
 * 
 * @throw {@link DatabaseException} when can't open database.
 */
void openDatabase();

/**
 * Close the database connection.
 *
 * @throw {@link DatabaseException} when can't close database.
 */
void closeDatabase();

/**
 * Create a media table inside the database. Should only be called when
 * there is no table.
 */
void createDatabaseTable();

/**
 * Execute a `SELECT` statement that will fetch ALL data of the MEDIA table.
 */
nlohmann::json selectAllJsonQuery();

/**
 * Execute a `SELECT` statement that will only fetch custom data. This is part of
 * private API.
 */
nlohmann::json _selectJsonQuery(const std::string &statement,
    int (*callback)(void*, int, char**, char **));

void addMedia(const media::Media &new_media);

void rmMedia(int rm_id);

void editMedia(int edit_id, const media::Media &edit_media);

#endif
