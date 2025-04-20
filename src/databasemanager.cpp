#include <sstream>

#include "databasemanager.h"
#include "media.h"
#include "debug/debprint.h"

namespace
{
    std::string LIB_DB_PATH;

    sqlite3 *_lib_db;

    /**
     * For a simple explanation how the callback works see:
     * https://stackoverflow.com/questions/31146713/sqlite3-exec-callback-function-clarification
     */
    int callback(void *_NOTUSED, int num_row, char **row_data, char **col_name) 
    {
        for(int i = 0; i < num_row; i++) 
        {
            printf("%s = %s", col_name[i], row_data[i] ? row_data[i] : "NULL");
            if (i != num_row - 1)
                printf(", ");
        }
        printf("\n");
        return 0;
    }

    bool execute_sql(const std::string &sql)
    {
        char *err_msg = 0;
        int ret = sqlite3_exec(_lib_db, sql.c_str(), callback, 0, &err_msg);
        if(ret != SQLITE_OK)
        {
            debug::print::fdeberr("SQL error: %s", err_msg);
            sqlite3_free(err_msg);
            return false;
        } 
        debug::print::fdebprint("SQL statement executed: %s", sql.c_str());
        return true;
    }
}

void initDatabase(const std::string &db_path, bool exist)
{
    LIB_DB_PATH = db_path;
    // Create db only when starting for the first time so
    // the table creation process can be skiped
    if (exist)
    {
        debug::print::deberr("Database already exists");
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

    ret = sqlite3_open(LIB_DB_PATH.c_str(), &_lib_db);

    if(ret)
    {
        debug::print::fdebprint("Can't open database: %s\n", sqlite3_errmsg(_lib_db));
    }
    else
    {
        debug::print::debprint("Opened database successfully\n");
    }
}

void closeDatabase()
{
    sqlite3_close(_lib_db);
    debug::print::debprint("Database closed");
}

void createDatabaseTable()
{
    const char *sql = "CREATE TABLE IF NOT EXISTS MEDIA("  \
       "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
       "NAME           TEXT    NOT NULL," \
       "RATING         INT     NOT NULL);";
 
    execute_sql(sql);
}

void showMedia()
{
    const char *sql = "SELECT * from MEDIA";
    execute_sql(sql);
}

void addMedia(const Media &new_media)
{
    // scary SQL injection BOOO 
    std::ostringstream oss;
    oss << "INSERT INTO MEDIA (NAME,RATING) VALUES('"
        << new_media._name << "',"
        << new_media._rating << ");";

    execute_sql(oss.str());
}

void rmMedia(int rm_id)
{
    std::ostringstream oss;
    oss << "DELETE FROM MEDIA WHERE ID="
        << rm_id << ";";
 
    execute_sql(oss.str());
}
