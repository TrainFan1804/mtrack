#include <iostream>

#include "optionparser.h"
#include "databasemanager.h"
#include "storage/utils.h"
#include "globals.h"

int main(int argc, char *argv[])
{
    createPath(APPDATA_PATH);
    if (log_active)
        createPath(LOG_PATH);
    initDatabase();
    try
    {
        po::parse(argc, argv);
    }
    catch(const std::exception &ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }
}
