#include <iostream>

#include "optionparser.h"
#include "databasemanager.h"
#include "debug/debprint.h"
#include "gui/launcher.h"

int main(int argc, char *argv[])
{
    initDatabase();
    debug::print::debprint("Start software");
    if (argc <= 1)
    {
        try
        {
            debug::print::debprint("Open gui");
            openDatabase();
            launch(argc, argv);
        }
        catch(const std::exception& e)
        {
            debug::print::debprint(e.what(), debug::DEBUG_LEVEL::ERROR);
            return EXIT_FAILURE;
        }
        closeDatabase();
        debug::print::debprint("Exit sofware. Close GUI.");
        return EXIT_SUCCESS;
    }

    try
    {
        debug::print::debprint("Start CLI mode");
        po::parse(argc, argv);
    }
    catch(const std::exception &ex)
    {
        debug::print::debprint(ex.what(), debug::DEBUG_LEVEL::ERROR);
        debug::print::debprint("Exit sofware. Close CLI.");
        return EXIT_FAILURE;
    }
}
