#include <iostream>

#include "optionparser.h"
#include "databasemanager.h"
#include "debug/debprint.h"
#include "gui/launcher.h"

int main(int argc, char *argv[])
{
    initDatabase();
    if (argc <= 1)
    {
        debug::print::debprint("Start software in GUI mode");
        try
        {
            // launchGUI();
            launch(argc, argv);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
        debug::print::debprint("Exit sofware. Close GUI.");
        return EXIT_SUCCESS;
    }

    try
    {
        debug::print::debprint("Start software in CLI mode");
        po::parse(argc, argv);
    }
    catch(const std::exception &ex)
    {
        debug::print::deberr(ex.what());
        debug::print::debprint("Exit sofware. Close CLI.");
        return EXIT_FAILURE;
    }
}
