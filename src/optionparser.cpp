#include "external/cxxopts.hpp"

#include "optionparser.h"
#include "db/database_service.h"
#include "globals/globals.h"
#include "globals/sql_globals.h"
#include "Media.h"
#include "debug/debprint.h"
#include "mtrack_exception/CLIException.h"

void po::parse(int argc, char *argv[])
{
    cxxopts::Options options(argv[0], "mtrack: A simple tool to track your media consume");

    options
      .add_options()
        ("s,show", "Show the content of the libary")
        ("a,add", "Add a new media to your libary",
            cxxopts::value<std::vector<std::string>>(),
            "<NAME> <RATING> <STATE> <TYPE>")
        ("r,remove", "Remove a media by id",
            cxxopts::value<int>(),
            "<id>")
        ("e,edit", "Edit a entry with the new parameter",
            cxxopts::value<std::vector<std::string>>(),
            "<EDITED_ID> <NEW_NAME> <NEW_RATING> <NEW_STATE> <NEW_TYPE>")
    ;
    options
        .add_options("misc")
        ("verbose", "Activate log")
        ("v,version", "Show version")
        ("h,help", "Show this page")
    ;

    // this isn't supported by `cxxopts` right now and might never be supported
    // like I imagine
    // options.parse_positional({"add", "edit"});
    auto result = options.parse(argc, argv);
    
    if (result.count("verbose"))
    {
      debug::setState(true);
    }
    if (result.count("help"))
    {
        printf(options.help({"", "misc"}).c_str());
        return;
    }
    if (result.count("version"))
    {
        printf("mTrack: %s\n", std::string(VERSION).c_str());
        return;
    }
    if (result.count("show"))
    {
        commands::showCommand();
    }
    if (result.count("add"))
    {
        auto args = result["add"].as<std::vector<std::string>>();
        if (args.size() != 4)
        {
            throw mtrack::CLIException("To few/many options for add");
        }
        commands::addCommand(args);
    }
    if (result.count("remove"))
    {
        commands::rmCommand(result["remove"].as<int>());
    }
    if (result.count("edit"))
    {
        auto args = result["edit"].as<std::vector<std::string>>();
        int edit_id = std::stoi(args[0]);
        args.erase(args.begin());
        commands::editCommand(edit_id, args);
    }
}

void po::commands::showCommand()
{
    const std::vector<std::string> EXPECTED_COLS = { TABLE_ALL_COL };   
    auto json = selectAllJsonQuery(); 

    for (const auto &item : json)
    {
        printf("id: %d, ", item["id"].get<int>());
        printf("name: %s, ", item["name"].get<std::string>().c_str());
        printf("rating: %d, ", item["rating"].get<int>());
        printf("state: %s, ", item["state"].get<std::string>().c_str());
        printf("type: %s\n", item["type"].get<std::string>().c_str());
    }
}

void po::commands::addCommand(const std::vector<std::string> &args)
{
    nlohmann::json json;
    json["name"] = args[0];
    json["rating"] = std::stoi(args[1]);
    json["state"] = args[2];
    json["type"] = args[3];
    media::Media new_media(json);
    addMedia(new_media);
}

void po::commands::rmCommand(int id)
{
    rmMedia(id);
}

void po::commands::editCommand(int edit_id, const std::vector<std::string> &args)
{
    nlohmann::json json;
    json["name"] = args[0];
    json["rating"] = std::stoi(args[1]);
    json["state"] = args[2];
    json["type"] = args[3];
    media::Media edit_media(json);
    editMedia(edit_id, edit_media);
}
