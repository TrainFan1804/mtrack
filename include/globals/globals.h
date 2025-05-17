#ifndef GLOBALS_H 
#define GLOBALS_H

#include <string>

#include "buildenv.h"

#ifndef VERSION
#define VERSION "unknown"
#endif

#define LOG_NAME "debug.log"

inline const std::string LOG_PATH_STR = std::string(LOG_DIR_PATH)
    + "/" +  std::string(LOG_NAME);

inline bool log_active = false;

#endif
