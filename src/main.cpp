#include <iostream>

#include "optionparser.h"
#include "databasemanager.h"
// #include "storage/utils.h"
// #include "globals.h"
#include "guilauncher.h"

int main(int argc, char *argv[])
{
    initDatabase();

    if (argc <= 1)
    {
        launchGUI();
        // auto exec_python_gui = std::string(PYTHON_PATH) + " " + std::string(GUI_PY);
        // int ret = std::system(exec_python_gui.c_str());
        // if (ret != 0)
        // {
        //     std::cerr << "Something went wrong trying open the gui.\n";
        //     return ret;
        // }
        // return 0;
    }

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
