#include "db/extractor/IDatabaseExtractor.h"
#include "db/extractor/SqlExtractor.h"

std::unique_ptr<IDatabaseExtractor> createExtractor(const std::string &type) 
{
    if (type == "sql") return std::make_unique<SqlExtractor>();
    return nullptr;
}
