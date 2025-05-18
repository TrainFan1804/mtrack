// Everything that will work with the filesystem directly 
// (e.g. open a file, get a path, etc.) should be in this directory.
// Stuff for database connections, etc. is NOT in this directory.

#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "utils/storage.h"

bool mtrack::fileIsCreated(const std::string &path)
{
    return std::filesystem::exists(path);
}
