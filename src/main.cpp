#include <iostream>

#include "databasemanager.h"
#include "debug/debprint.h"
#include "gui/launcher.h"

int main(int argc, char *argv[])
{
    debug::print::debprint("Start software");
    initDatabase();
    try
    {
        openDatabase();
        launch(argc, argv);
    }
    catch(const std::exception &ex)
    {
        debug::print::deberr(ex.what());
        return EXIT_FAILURE;
    }
    closeDatabase();
    debug::print::debprint("Close sofware");
    return EXIT_SUCCESS;
}
