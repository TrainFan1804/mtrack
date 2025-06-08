#pragma once

#include <memory>
#include <string>

struct sqlite3;

class IDatabaseImporter
{
public:
    virtual ~IDatabaseImporter() {};
    virtual void importDatabase(sqlite3 *db, const std::string &dump_file) = 0;
};

std::unique_ptr<IDatabaseImporter> createImporter(const std::string &type);
