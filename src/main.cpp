#include <iostream>

#include "optionparser.h"
#include "databasemanager.h"
#include "filesystem/utils.h"

int main(int argc, char *argv[])
{
    auto path = getDBPath();
    bool exist = isCreated(path);
    initDatabase(path, exist);
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
