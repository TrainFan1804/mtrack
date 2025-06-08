#ifndef STORAGE_H
#define STORAGE_H

#include <string>

namespace mtrack
{
    /**
     * Check if a file exists in the given path
     */
    bool fileIsCreated(const std::string &path);
} // namespace mtrack

#endif
