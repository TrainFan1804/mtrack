#include "db/database_service.h"

#include <sstream>

#include "db/custom_callbacks.h"
#include "db/database_manager.h"
#include "debug/debprint.h"
#include "globals/sql_globals.h"
#include "utils/time.h"

namespace
{
    sqlite3 *_db;
}

void initDatabase()
{
    initDatabase(_db);
}

void openDatabase()
{
    openDatabase(&_db);
    debug::print::debprint("Opened database successfully", debug::INFO);
}

void closeDatabase()
{
    closeDatabase(_db);
    debug::print::debprint("Database closed", debug::INFO);
}

void addMedia(const media::Media &new_media)
{
    // scary SQL injection BOOO
    std::ostringstream oss;
    oss << "INSERT INTO " << TABLE_NAME << " (" << new_media.attributesToSql()
        << ") VALUES(" << new_media.valuesToSql() << ");";
    execute_sql(_db, oss.str());
}

void rmMedia(int rm_id)
{
    std::ostringstream oss;
    oss << "DELETE FROM " << TABLE_NAME << " WHERE ID=" << rm_id << ";";
    execute_sql(_db, oss.str());
}

void editMedia(int edit_id, const media::Media &edit_media)
{
    std::ostringstream oss;
    oss << "UPDATE " << TABLE_NAME << " SET "
        << " NAME = " << "'" << edit_media._name << "', "
        << " RATING  =" << "'" << edit_media._rating << "', "
        << " STATE = " << "'" << edit_media._state << "', "
        << " TYPE = " << "'" << edit_media._type << "'"
        << " WHERE ID = " << edit_id << ";";
    execute_sql(_db, oss.str());
}

nlohmann::json selectAllJsonQuery()
{
    std::string result;
    execute_sql(_db, SQL_JSON_SELECT_ALL, &result, jsonSelectCallback);
    auto j = nlohmann::json::parse(result);
    debug::print::debprintf(debug::INFO, "Data fetched: {}", result);
    return j;
}

void dumpDatabase(IDatabaseExtractor *extractor)
{
    debug::print::debprint("Start dumping database to file...");

    std::string time_stamp = mtrack::getCustomCurrentTimestamp("%Y-%m-%d");
    std::string dump_file  = std::string(BACKUP_DIR_PATH) + "/" + time_stamp;

    extractor->exportDatabase(_db, dump_file);
    debug::print::debprint("Dumping database to file complete");
}

void importDatabase(IDatabaseImporter *importer, const std::string &import_file)
{
    debug::print::debprint("Start importing database...");
    std::string sql_dropatable = "DROP TABLE IF EXISTS MEDIA";
    execute_sql(_db, sql_dropatable);
    importer->importDatabase(_db, import_file);

    debug::print::debprint("Importing database complete");
}
