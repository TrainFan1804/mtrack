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
    int callback(void *head, int num_cols, char **row_data, char **col_name) 
    {
        auto &h = *(reinterpret_cast<std::vector<std::vector<std::string>>*>(head));

        std::vector<std::string> row;
        // Double the size of the columns to save the column name too.
        // Could be obsolete when just show the columns in the front end.

        // TODO remove the column names from the vector -> display them in the frontend
        // directly
        row.reserve(num_cols * 2);
        for (int i = 0; i < num_cols; i++)
        {
            row.emplace_back(col_name[i]);
            row.emplace_back(row_data[i] ? row_data[i] : "NULL");
        }
        h.emplace_back(std::move(row));
        return 0;
    }

    /**
     * Execute a sql statement. Only use this directly when you now what you do.
     * 
     * @attention   Rather use execute_sql(const std::string&) 
     * or execute_sql(const std::string&, std::vector<std::vector<std::string>>&)
     */
    void execute_sql(const std::string &sql, void *head)
    {
        char *err_msg = 0;
        int ret = sqlite3_exec(_lib_db, sql.c_str(), callback, head, &err_msg);
        if(ret != SQLITE_OK)
        {
            debug::print::fdeberr("SQL error: %s", err_msg);
            sqlite3_free(err_msg);
            throw std::runtime_error(err_msg);
        } 
        debug::print::fdebprint("SQL statement executed: %s", sql.c_str());
    }

    /**
     * Execute a sql statement. This should be used when working with NON
     * `SELECT` statements.
     */
    void execute_sql(const std::string &sql)
    {
        execute_sql(sql, nullptr);
    }

    /**
     * Execute a sql statement. This should only be used when working with
     * `SELECT` statements.
     * 
     * @param sql   The executed sql statement. This function should only be used with
     * `SELECT` statements!
     * @param head  When executing a `SELECT` statment you need to provide a container
     * to save the response from the query.
     */
    void execute_sql(const std::string &sql, 
        std::vector<std::vector<std::string>> &head)
    {
        execute_sql(sql, &head);
    }
}

void initDatabase()
{
    // Create db only when starting for the first time so
    // the table creation process can be skiped
    if (fileIsCreated(DB_PATH_STR))
    {
        debug::print::debprint("Database already exists. Skip table creation.");
        checkTable();
        return;
    }
    openDatabase();
    debug::print::debprint("Database created");
    createDatabaseTable();
    debug::print::debprint("Tables created");
    closeDatabase();
}

void checkTable()
{
    openDatabase();
    std::ostringstream oss;
    oss << "PRAGMA table_info(" 
        << TABLE_NAME << ");";
    
    std::vector<std::vector<std::string>> select_result;
    selectSpecialQuery(select_result, oss.str());

    std::vector<std::string> real_cols; // cols in the actuall table
    
    for (const auto &row : select_result)
    {
        real_cols.push_back(row[3]);
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
        debug::print::fdeberr("Can't open database: %s", sqlite3_errmsg(_lib_db));
    }
    else
    {
        debug::print::debprint("Opened database successfully");
    }
}

void closeDatabase()
{
    sqlite3_close(_lib_db);
    debug::print::debprint("Database closed");
}

void createDatabaseTable()
{
    execute_sql(SQL_CREATE);
}

void selectAllQuery(std::vector<std::vector<std::string>> &result)
{
    selectSpecialQuery(result, "SELECT * FROM " + std::string(TABLE_NAME) + ";");
}

void selectSpecialQuery(std::vector<std::vector<std::string>> &result,
    const std::string &statement)
{
    execute_sql(statement.c_str(), result);
}

nlohmann::json selectAllJsonQuery()
{
    std::vector<std::vector<std::string>> select_result;
    execute_sql(SQL_JSON_SELECT_ALL, select_result);

    auto raw_str = select_result[0][1];
    auto j = nlohmann::json::parse(raw_str);
    return j;
}

nlohmann::json selectJsonQuery(const std::string &statement)
{
    std::vector<std::vector<std::string>> select_result;
    execute_sql(statement.c_str(), select_result);

    auto raw_str = select_result[0][1];
    auto j = nlohmann::json::parse(raw_str);
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
