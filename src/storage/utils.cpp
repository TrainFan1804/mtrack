#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "storage/utils.h"

void createPath(const std::filesystem::path &path)
{
    std::filesystem::create_directory(path);
}

bool fileIsCreated(const std::string &path)
{
    return std::filesystem::exists(path);
}
