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

void restoreDatabase(IDatabaseImporter *importer, const std::string &retore_file);
