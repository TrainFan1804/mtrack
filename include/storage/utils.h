#ifndef UTILS_H
#define UTILS_H

#include <string>

/** 
 * Create a directory in the filesystem
 * 
 * @deprecated This was used in the past to create APPLICATION_PATH and LOG_PATH.
 * The creation process was moved into build.sh. Will propably be removed in the future.
 */
void createPath(const std::filesystem::path &path);

/**
 * Check if a file exists in the given path
 */
bool fileIsCreated(const std::string &path);

#endif
