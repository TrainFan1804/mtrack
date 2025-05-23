#include <ostream>
#include <fstream>

#include <sqlite3.h>

#include "db/extractor/DumpExtractor.h"
#include "buildenv.h"
#include "debug/debprint.h"
#include "utils/time.h"

void DumpExtractor::exportDatabase(sqlite3 *db)
{
    debug::print::debprint("Start dumping database as backup...");

    std::string time_stamp = mtrack::getCustomCurrentTimestamp("%Y-%m-%d");
    std::ofstream off(std::string(BACKUP_DIR_PATH) 
        + "/" + time_stamp + ".dump");

    off << "----------------------------------------\n";
    off << "----    SQL dump from " << time_stamp << "    ----\n";
    off << "----------------------------------------\n";
    off << "PRAGMA foreign_keys=OFF;\n";
    off << "BEGIN TRANSACTION;\n";

    std::string schema_query = "SELECT sql FROM sqlite_master WHERE type='table' AND name='MEDIA';";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, schema_query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
        if (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            const char *sql = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            off << sql << ";\n";
        }
    }
    sqlite3_finalize(stmt);

    std::string data_query = "SELECT * FROM MEDIA;";
    if (sqlite3_prepare_v2(db, data_query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
        int col_count = sqlite3_column_count(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            off << "INSERT INTO MEDIA VALUES(";
            for (int i = 0; i < col_count; ++i)
            {
                if (i > 0) off << ", ";
                switch (sqlite3_column_type(stmt, i)) 
                {
                case SQLITE_INTEGER:
                    off << sqlite3_column_int64(stmt, i);
                    break;
                case SQLITE_TEXT:
                    off << "'" << sqlite3_column_text(stmt, i) << "'";
                    break;
                case SQLITE_NULL:
                    off << "NULL";
                    break;
                default:
                    off << "'[UNSUPPORTED]'";
                    break;
                }
            }
            off << ");\n";
        }
    }
    sqlite3_finalize(stmt);
    off << "COMMIT;\n";
    off.close();

    debug::print::debprint("Dumping complete");
}
