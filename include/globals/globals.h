#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#include "buildenv.h"

#ifndef VERSION
    #define VERSION "unknown"
#endif

inline const std::string LOG_BASE_PATH = std::string(LOG_DIR_PATH);

#endif
