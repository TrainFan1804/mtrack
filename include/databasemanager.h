#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>

#include <sqlite3.h>

struct Media;

void initDatabase(const std::string &db_path, bool exist);

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

void showMedia();
void addMedia(const Media &new_media);
void rmMedia(int rm_id);

#endif
