#include "db/importer/SqlImporter.h"

#include <fstream>
#include <sqlite3.h>

#include "db/database_manager.h"

void SqlImporter::importDatabase(sqlite3 *db, const std::string &dump_file)
{
    std::ifstream in(dump_file);

    std::string sql;
    std::string line;

    while (std::getline(in, line))
    {
        sql += line;
        if (line.find(';') != std::string::npos)
        {
            execute_sql(db, sql.c_str());
            sql.clear();
        }
    }
    in.close();
}
