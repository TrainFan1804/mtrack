#include <stdexcept>

#include "cxxopts.hpp"

#include "optionparser.h"
#include "databasemanager.h"
#include "globals.h"
#include "debug/debprint.h"

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
        ("a,add", "Add a new media to your libary", cxxopts::value<std::vector<std::string>>(), "<NAME> <RATING> <STATE>")
        ("r,remove", "Remove a media by id", cxxopts::value<int>(), "<id>")
        ("verbose", "Activate log")
        ("v,version", "Show version")
        ("h,help", "Show this page")
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
        return;
    }
    if (result.count("version"))
    {
        printf("mTrack: %s\n", std::string(VERSION).c_str());
        return;
    }
    openDatabase();
    if (result.count("show"))
    {
        commands::showCommand();
    }
    if (result.count("add"))
    {
        if (result.count("add") != 3)
        {
            throw std::runtime_error("To few/many options for add");
        }
        commands::addCommand(result["add"].as<std::vector<std::string>>());
    }
    if (result.count("remove"))
    {
        commands::rmCommand(result["remove"].as<int>());
    }
    closeDatabase();
}

void po::commands::showCommand()
{
    std::vector<std::vector<std::string>> select_result;
    selectAllQuery(select_result);
    for (auto row : select_result)
    {
        for (int entry = 0; entry < row.size(); entry++)
        {
            printf(row[entry].c_str());
            if (entry != row.size() - 1) printf((entry % 2 == 0) ? " = " : ", ");
        }
        printf("\n");
    }
}

void po::commands::addCommand(const std::vector<std::string> &args)
{
    media::Media new_media(args[0], std::stoi(args[1]), args[2]);
    addMedia(new_media);
}

void po::commands::rmCommand(int id)
{
    rmMedia(id);
}
