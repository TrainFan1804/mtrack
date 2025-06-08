#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>

#include "Media.h"
#include "external/json.hpp"

void initDatabase(sqlite3 *db);

/**
 * This functions is called when the user is updating `mtrack` to a new
 * version that require tables that aren't in older version.
 */
void checkTableCompletion(sqlite3 *db);

/**
 * Open the database connection.
 *
 * @throw `DatabaseException` when can't open database.
 */
void openDatabase(sqlite3 **db);

/**
 * Close the database connection.
 *
 * @throw `DatabaseException` when can't close database.
 */
void closeDatabase(sqlite3 *db);

/**
 * Create a media table inside the database. Should only be called when
 * there is no table.
 */
void createDatabaseTable(sqlite3 *db);

/**
 * Use this when you want to add a custom callback to your SQL querry.
 *
 * @throw `DatabaseException` when a SQL error occure.
 */
void execute_sql(
    sqlite3 *db, const std::string &sql, void *head,
    int (*callback)(void *, int, char **, char **)
);

/**
 * Execute a sql statement. This should be used when working with NON
 * `SELECT` statements.
 */
void execute_sql(sqlite3 *db, const std::string &sql);

#endif
