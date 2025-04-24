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
        return;
    }
    openDatabase();
    debug::print::debprint("Database created");
    createDatabaseTable();
    debug::print::debprint("Tables created");
    closeDatabase();
}

void openDatabase()
{
    char *err_msg = 0;
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
    const char *sql = "CREATE TABLE IF NOT EXISTS MEDIA("   \
       "ID          INTEGER     PRIMARY KEY,"               \
       "NAME        TEXT        NOT NULL,"                  \
       "RATING      INT         NOT NULL"                   \
    ");";
 
    execute_sql(sql);
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
    const std::string NAME_LIST[] = { TABLE_COL };

    std::ostringstream oss;
    oss << "SELECT json_group_array(json_object(";

    // why does NAME_LIST->length() return ACTUALLY_LENGTH - 1?
    for (int index = 0; index <= NAME_LIST->length(); index++)
    {
        oss << "'" << strToLower(NAME_LIST[index]) << "', " << NAME_LIST[index];
        if (index != NAME_LIST->length())
            oss << ", ";
    }
    oss << ")) AS json_result FROM " << TABLE_NAME << ";";

    std::vector<std::vector<std::string>> select_result;
    execute_sql(oss.str(), select_result);

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
        << " (NAME,RATING) VALUES('"
        << new_media._name << "',"
        << new_media._rating 
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
