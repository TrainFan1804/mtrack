#include "db/importer/IDatabaseImporter.h"
#include "db/importer/SqlImporter.h"

std::unique_ptr<IDatabaseImporter> createImporter(const std::string &type) 
{
    if (type == "sql") return std::make_unique<SqlImporter>();
    return nullptr;
}
