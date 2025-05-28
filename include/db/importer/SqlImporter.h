#pragma once

#include <string>

#include "db/importer/IDatabaseImporter.h"

class SqlImporter : public IDatabaseImporter
{
public:
    void importDatabase(sqlite3 *db, const std::string &dump_file) override;
};
