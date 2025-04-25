#ifndef DEBPRINT_H
#define DEBPRINT_H

#include <string>

namespace debug
{
    enum DEBUG_LEVEL
    {
        INFO, ERROR
    };
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
    void debprint(const std::string &msg);

    void deberr(const std::string &err);

    void fdebprint(const char *msg, ...);

    void fdeberr(const char *err, ...);
}

#endif
