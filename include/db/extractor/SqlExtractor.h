#pragma once

#include "db/extractor/IDatabaseExtractor.h"

class SqlExtractor : public IDatabaseExtractor
{
public:
    void exportDatabase(sqlite3 *db, const std::string &dump_file) override;
};
