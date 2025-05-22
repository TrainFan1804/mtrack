#ifndef DEBPRINT_H
#define DEBPRINT_H

#include <string>

#include "external/fmt/format.h"

namespace debug
{
    enum DEBUG_LEVEL
    {
        ERROR,
        WARNING,
        INFO,
    };

    /**
     * Call this function to de-/activate the logger. Will also handle
     * log file opening and closing.
     */
    void setState(bool state);
    std::string toString(DEBUG_LEVEL level);
}

/**
 * All functions in this namespace will provice output that is determined to debug
 * and logging purpose. Loggin will only be activated when `mTrack --verbose` active
 * and normal console output is only in dev mode activated.
 * Errors will ALWAYS be displayed regardless of mode.
 * 
 * For output that should be showed regardless of the activated mode just use the
 * default options C++ provide.
 */
namespace debug::print
{
    void debprint(const std::string &msg, DEBUG_LEVEL level = INFO);

    /**
     * To print a message like `printf` use this function. To format the message
     * correctly `msg` need to look something like this: {@code "I am {} years old!"}
     * "{}" will be replaced by the inserted format.
     * 
     * When using multiple "{}" the values in `args` will be set from left to right.
     */
    template<typename... Args>
    void debprintf(DEBUG_LEVEL level, 
        const fmt::format_string<Args...> msg,
        Args&&... args)
    {
        std::string message = fmt::format(msg, std::forward<Args>(args)...);
        debug::print::debprint(message, level);
    }
}

#endif
