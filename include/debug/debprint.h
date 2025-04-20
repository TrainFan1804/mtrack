#ifndef DEBPRINT_H
#define DEBPRINT_H

#include <string>

namespace debug
{
    enum DEBUG_LEVEL
    {
        INFO, WARNING, ERROR
    };

    inline bool log_active = false;
}

namespace debug::print
{
    void debprint(const std::string &msg);

    void deberr(const std::string &err);

    void fdebprint(const char *msg, ...);

    void fdeberr(const char *err, ...);
}

#endif
