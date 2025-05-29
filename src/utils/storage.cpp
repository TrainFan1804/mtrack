// Everything that will work with the filesystem directly
// (e.g. open a file, get a path, etc.) should be in this file.
// Stuff for database connections, etc. is NOT in this file.

#include "utils/storage.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>

bool mtrack::fileIsCreated(const std::string &path)
{
    return std::filesystem::exists(path);
}
