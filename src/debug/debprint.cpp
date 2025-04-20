#include <iostream>
#include <cstdarg>
#include <cstdio>

#include "debug/debprint.h"

namespace
{
    void log(const std::string &msg, debug::DEBUG_LEVEL)
    {
        if (!debug::log_active)
            return;

        std::cout << "Log isn't implemented yet.\n";
    }
}

void debug::print::debprint(const std::string &msg)
{
    #ifdef DEBUG
        std::cout << msg << "\n";
    #endif
}

void debug::print::deberr(const std::string &err)
{
    #ifdef DEBUG
        std::cerr << err << "\n";
    #endif
}

void debug::print::fdebprint(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), msg, args);

    va_end(args);
    debprint(std::string(buffer));
}

void debug::print::fdeberr(const char *err, ...)
{
    va_list args;
    va_start(args, err);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), err, args);

    va_end(args);
    deberr(std::string(buffer));
}
