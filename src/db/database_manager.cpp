#include <sstream>
#include <vector>

#include "db/database_manager.h"
#include "db/custom_callbacks.h"
#include "globals/sql_globals.h"
#include "mtrack_exception/DatabaseException.h"
#include "debug/debprint.h"
#include "utils/storage.h"

namespace
{
    /**
     * Execute a `SELECT` statement that will only fetch custom data.
     */
    nlohmann::json _selectJsonQuery(sqlite3 *db, const std::string &statement, 
        int (*callback)(void*, int, char**, char **))
    {
        std::string result;
        execute_sql(db, statement.c_str(), &result, callback);
        auto j = nlohmann::json::parse(result);
        debug::print::debprintf(debug::INFO, "Data fetched: {}", result);
        return j;
    }
}

void initDatabase(sqlite3 *db)
{
    // Create db only when starting for the first time so
    // the table creation process can be skiped
    if (mtrack::fileIsCreated(DB_PATH_STR))
    {
        debug::print::debprint(
            "Database already exists. Skip table creation.",
            debug::WARNING
        );
        checkTableCompletion(db);
        return;
    }
    openDatabase(&db);
    debug::print::debprint("Database created");
    createDatabaseTable(db);
    debug::print::debprint("Tables created");
    closeDatabase(db);
}

void checkTableCompletion(sqlite3 *db)
{
    debug::print::debprint("Check if table in database contains all required columns...");
    openDatabase(&db);
    std::ostringstream oss;
    oss << "SELECT json_group_array(json_object('col_name', NAME)) AS json_result FROM pragma_table_info('MEDIA');";
    
    auto media_col_names = _selectJsonQuery(db, oss.str(), jsonSelectCallback);

    std::vector<std::string> real_cols; // cols in the actuall table
    
    for (const auto &item: media_col_names)
    {
        real_cols.push_back(item["col_name"].get<std::string>());
    }
    
    // cols that should be in the table
    const std::vector<std::string> EXPECTED_COLS = { TABLE_ALL_COL };   

    for (auto &ex_col : EXPECTED_COLS)
    {
        bool col_exists = false;
        for (auto &col: real_cols)
        {
            if (ex_col == col)
            {
                col_exists = true;
                break;
            }
        }
        if (!col_exists)
        {
            std::ostringstream oss;
            oss << "ALTER TABLE " << TABLE_NAME << " ADD COLUMN " << ex_col << " TEXT DEFAULT 'unknown';";
            execute_sql(db, oss.str());
        }
    }
    closeDatabase(db);
    debug::print::debprint("Table check complete");
}

void openDatabase(sqlite3 **db)
{
    int ret = sqlite3_open(DB_PATH_STR.c_str(), db);
    if(ret != SQLITE_OK)
    {
        throw mtrack::DatabaseException("Can't open database: " 
            + std::string(sqlite3_errmsg(*db)));
    }
}

void closeDatabase(sqlite3 *db)
{
    int ret = sqlite3_close(db);
    if (ret != SQLITE_OK)
    {
        throw mtrack::DatabaseException("Can't close database"
            + std::string(sqlite3_errmsg(db)));
    }
}

void createDatabaseTable(sqlite3 *db)
{
    execute_sql(db, SQL_CREATE);
}

void execute_sql(sqlite3 *db, const std::string &sql, void *head, 
    int (*callback)(void*, int, char**, char **))
{
    char *err_msg = 0;
    
    int ret = sqlite3_exec(db, sql.c_str(), callback, head, &err_msg);
    if(ret != SQLITE_OK)
    {
        std::string e = "SQL error: " + std::string(err_msg);
        sqlite3_free(err_msg);
        throw mtrack::DatabaseException(e);
    } 
    debug::print::debprintf(debug::INFO, "SQL statement executed: {}", sql);
}

void execute_sql(sqlite3 *db, const std::string &sql)
{
    execute_sql(db, sql, NULL, NULL);
}
