#include <stdexcept>

#include "cxxopts.hpp"

#include "optionparser.h"
#include "commands.h"
#include "databasemanager.h"
#include "globals.h"

void po::parse(int argc, char *argv[])
{
    std::unique_ptr<cxxopts::Options> allocated(new cxxopts::Options(argv[0], "example command line options"));

    auto& options = *allocated;
    options
      .positional_help("[optional args]")
      .show_positional_help();

    options
      .set_width(70)
      .set_tab_expansion()
      .allow_unrecognised_options()
      .add_options()
        ("s,show", "Show the content of the libary")
        ("a,add", "Add a new media to your libary", cxxopts::value<std::vector<std::string>>(), "<NAME> <RATING>")
        ("r,remove", "Remove a media by id", cxxopts::value<int>(), "<id>")
        ("verbose", "Activate log")
        ("v,version", "Show version")
        ("h,help", "Print help")
      ;

    options.parse_positional({"add"});
    auto result = options.parse(argc, argv);
    if (result.count("verbose"))
    {
      log_active = true;
    }
    if (result.count("help"))
    {
        printf(options.help().c_str());
    }
    if (result.count("version"))
    {
        auto temp = std::string("Current version: " + std::string(VERSION));
        printf(temp.c_str());
    }
    openDatabase();
    if (result.count("show"))
    {
        showCommand();
    }
    if (result.count("add"))
    {
        if (result.count("add") != 2)
        {
            throw std::runtime_error("To few options for add");
        }
        addCommand(result["add"].as<std::vector<std::string>>());
    }
    if (result.count("remove"))
    {
        rmCommand(result["remove"].as<int>());
    }
    closeDatabase();
}
