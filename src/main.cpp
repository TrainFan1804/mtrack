#include <iostream>

#include "db/database_service.h"
#include "debug/debprint.h"
#include "gui/launcher.h"
#include "optionparser.h"

int main(int argc, char *argv[])
{
    int ret;
    try
    {
        debug::print::debprint("Start software");
        initDatabase();
        openDatabase();
        if (argc <= 1)
        {
            debug::print::debprint("Open gui");
            launch(argc, argv);
            debug::print::debprint("Exit sofware. Close GUI.");
        }
        else
        {
            debug::print::debprint("Start CLI mode");
            po::parse(argc, argv);
            debug::print::debprint("Exit sofware. Close CLI.");
        }
        ret = EXIT_SUCCESS;
    }
    catch (const std::exception &ex)
    {
        debug::print::debprint(ex.what(), debug::DEBUG_LEVEL::ERROR);
        ret = EXIT_FAILURE;
    }
    closeDatabase();
    return ret;
}
