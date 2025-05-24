#include <fstream>

#include <sqlite3.h>

#include "db/importer/SqlImporter.h"

void SqlImporter::importDatabase(sqlite3 *db, const std::string &dump_file)
{
    std::ifstream in(dump_file);

    std::string sql;
    std::string line;
    char* errMsg = nullptr;

    while (std::getline(in, line))
    {
        sql += line + "\n";
        if (line.find(';') != std::string::npos) 
        {
            if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) 
            {
                sqlite3_free(errMsg);
            }
            sql.clear();
        }
    }
    in.close();
}
