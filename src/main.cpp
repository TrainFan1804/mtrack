#include <iostream>

#include "optionparser.h"
#include "databasemanager.h"
#include "guilauncher.h"
#include "debug/debprint.h"

int main(int argc, char *argv[])
{
    initDatabase();

    if (argc <= 1)
    {
        launchGUI();
        return 0;
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
