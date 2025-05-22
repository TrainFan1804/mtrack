#pragma once

#include "db/extractor/IDatabaseExtractor.h"

class DumpExtractor : public IDatabaseExtractor
{
public:
    virtual void exportDatabase(sqlite3 *db);
};
