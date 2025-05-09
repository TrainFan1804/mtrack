#include <sstream>
#include <vector>

#include "databasemanager.h"
#include "debug/debprint.h"
#include "storage/utils.h"

namespace
{
    sqlite3 *_lib_db;

    /**
     * For a simple explanation how the callback works see:
     * https://stackoverflow.com/questions/31146713/sqlite3-exec-callback-function-clarification
     * 
     * This function is called for every row in the table.
     * 
     * This is some crazy sh*t here
     */
    int jsonSelectCallback(void *head, int num_cols, char **row_data, char **NOT_USED) 
    {
        if (num_cols > 0 && row_data[0]) {
            std::string* result = static_cast<std::string*>(head);
            *result = row_data[0];
        } else {
            std::string* result = static_cast<std::string*>(head);
            *result = "[]";
        }
        return 0;
    }

    /**
     * Use this when you want to add a custom callback to your SQL querry.
     */
    void execute_sql(const std::string &sql, void *head, 
        int (*callback)(void*, int, char**, char **))
    {
        char *err_msg = 0;
        int ret = sqlite3_exec(_lib_db, sql.c_str(), callback, head, &err_msg);
        if(ret != SQLITE_OK)
        {
            debug::print::fdeberr("SQL error: %s", debug::DB, err_msg);
            sqlite3_free(err_msg);
            throw std::runtime_error(err_msg);
        } 
        debug::print::fdebprint("SQL statement executed: %s", debug::DB, sql.c_str());
    }

    /**
     * Execute a sql statement. This should be used when working with NON
     * `SELECT` statements.
     */
    void execute_sql(const std::string &sql)
    {
        execute_sql(sql, NULL, NULL);
    }
}

void initDatabase()
{
    // Create db only when starting for the first time so
    // the table creation process can be skiped
    if (fileIsCreated(DB_PATH_STR))
    {
        debug::print::debprint(
            "Database already exists. Skip table creation.",
            debug::DB
        );
        checkTable();
        return;
    }
    openDatabase();
    debug::print::debprint("Database created", debug::DB);
    createDatabaseTable();
    debug::print::debprint("Tables created", debug::DB);
    closeDatabase();
}

void checkTable()
{
    openDatabase();
    std::ostringstream oss;
    oss << "SELECT json_group_array(json_object('col_name', NAME)) AS json_result FROM pragma_table_info('MEDIA');";
    
    auto media_col_names = _selectJsonQuery(oss.str(), jsonSelectCallback);

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
            execute_sql(oss.str());
        }
    }
    closeDatabase();
}

void openDatabase()
{
    int ret;
    ret = sqlite3_open(DB_PATH_STR.c_str(), &_lib_db);

    if(ret)
    {
        debug::print::fdeberr(
            "Can't open database: %s", 
            debug::DB, 
            sqlite3_errmsg(_lib_db)
        );
    }
    else
    {
        debug::print::debprint("Opened database successfully", debug::DB);
    }
}

void closeDatabase()
{
    sqlite3_close(_lib_db);
    debug::print::debprint("Database closed", debug::DB);
}

void createDatabaseTable()
{
    execute_sql(SQL_CREATE);
}

nlohmann::json selectAllJsonQuery()
{
    std::string result;
    execute_sql(SQL_JSON_SELECT_ALL, &result, jsonSelectCallback);
    auto j = nlohmann::json::parse(result);
    debug::print::fdebprint("Data fetched: %s", debug::DB, result.c_str());
    return j;
}

nlohmann::json _selectJsonQuery(const std::string &statement, 
    int (*callback)(void*, int, char**, char **))
{
    std::string result;
    execute_sql(statement.c_str(), &result, callback);
    auto j = nlohmann::json::parse(result);
    debug::print::fdebprint("Data fetched: %s", debug::DB, result.c_str());
    return j;
}

void addMedia(const media::Media &new_media)
{
    // scary SQL injection BOOO 
    std::ostringstream oss;
    oss << "INSERT INTO " 
        << TABLE_NAME 
        << " (" 
        << new_media.attributesToSql() 
        << ") VALUES("
        << new_media.valuesToSql()
        << ");";
    execute_sql(oss.str());
}

void rmMedia(int rm_id)
{
    std::ostringstream oss;
    oss << "DELETE FROM "
        << TABLE_NAME 
        << " WHERE ID="
        << rm_id 
        << ";";
    execute_sql(oss.str());
}

void editMedia(int edit_id, const media::Media &edit_media)
{
    std::ostringstream oss;
    oss << "UPDATE " 
        << TABLE_NAME 
        << " SET " 
        << " NAME = " << "'" << edit_media._name << "', "
        << " RATING  ="<< "'" << edit_media._rating << "', "
        << " STATE = " << "'" << edit_media._state << "', "
        << " TYPE = " << "'" << edit_media._type << "'"
        << " WHERE ID = " << edit_id << ";";
    execute_sql(oss.str());
}
