#pragma once

#include "Media.h"
#include "db/extractor/IDatabaseExtractor.h"
#include "db/importer/IDatabaseImporter.h"

void initDatabase();

void openDatabase();

void closeDatabase();

void addMedia(const media::Media &new_media);

void rmMedia(int rm_id);

void editMedia(int edit_id, const media::Media &edit_media);

/**
 * Execute a `SELECT` statement that will fetch ALL data of the MEDIA table.
 */
nlohmann::json selectAllJsonQuery();

void dumpDatabase(IDatabaseExtractor *extractor);

/**
 * This will import the database with a selected `IDatabaseImporter` and the
 * path of the file that should be restored.
 * 
 * @warning This will delete all entries in the current database.
 */
void importDatabase(IDatabaseImporter *importer, const std::string &import_file);
