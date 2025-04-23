#include <iostream>

#include "optionparser.h"
#include "databasemanager.h"
#include "gui/guilauncher.h"
#include "debug/debprint.h"

int main(int argc, char *argv[])
{
    initDatabase();

    if (argc <= 1)
    {
        try
        {
            launchGUI();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    try
    {
        po::parse(argc, argv);
    }
    catch(const std::exception &ex)
    {
        debug::print::deberr(ex.what());
        closeDatabase();
        return EXIT_FAILURE;
    }
}
