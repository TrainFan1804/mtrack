#pragma once

struct sqlite3;

class IDatabaseExtractor
{
public:
    virtual ~IDatabaseExtractor() {}
    virtual void exportDatabase(sqlite3 *db) = 0;
};
