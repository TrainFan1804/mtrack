#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "filesystem/utils.h"

#define DB_NAME "mtrack.db"

std::string getDBPath()
{
    const std::string base_dir = std::string(std::getenv("HOME"))
        + "/.local/share";

    std::filesystem::path full_dir_path = base_dir + "/mtrack";
    std::filesystem::create_directories(full_dir_path);

    return (full_dir_path / DB_NAME).string();
}

bool isCreated(std::string &path)
{
    return std::filesystem::exists(path);
}
