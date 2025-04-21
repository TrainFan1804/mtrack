#ifndef UTILS_H
#define UTILS_H

#include <string>

/** 
 * Create a directory in the filesystem
 */
void createPath(const std::filesystem::path &path);

/**
 * Check if a file exists in the given path
 */
bool fileIsCreated(const std::string &path);

#endif
