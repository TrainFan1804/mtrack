#include "db/extractor/SqlExtractor.h"

#include <fstream>
#include <sqlite3.h>

#include "debug/debprint.h"
#include "globals/sql_globals.h"

void SqlExtractor::exportDatabase(sqlite3 *db, const std::string &dump_file)
{
    auto          d_file = dump_file + ".sql";
    std::ofstream off(d_file);

    // off << "----------------------------------------\n";
    // off << "----    SQL dump from " << time_stamp << "    ----\n";
    // off << "----------------------------------------\n";
    // off << "PRAGMA foreign_keys=OFF;\n";
    // off << "BEGIN TRANSACTION;\n";

    off << SQL_CREATE << "\n";

    sqlite3_stmt *stmt;
    std::string   data_query = "SELECT * FROM MEDIA;";
    if (sqlite3_prepare_v2(db, data_query.c_str(), -1, &stmt, nullptr)
        == SQLITE_OK)
    {
        int col_count = sqlite3_column_count(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            off << "INSERT INTO MEDIA VALUES(";
            for (int i = 0; i < col_count; ++i)
            {
                if (i > 0)
                    off << ", ";
                if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                    off << "'";
                off << sqlite3_column_text(stmt, i);
                if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                    off << "'";
            }
            off << ");\n";
        }
    }
    sqlite3_finalize(stmt);
    // off << "COMMIT;\n";
    off.close();
}
