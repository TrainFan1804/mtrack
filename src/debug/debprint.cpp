#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <fstream>

#include "debug/debprint.h"
#include "utils/time.h"
#include "globals.h"

namespace
{
    std::ofstream _log_file;

    void log(const std::string &msg, debug::DEBUG_LEVEL status)
    {
        if (!log_active)
            return;

        _log_file.open(LOG_PATH_STR, std::ios::app);
        if (!_log_file.is_open()) throw std::runtime_error("Couldn't open log file");

        std::string timestamp = mtrack::getCustomCurrentTimestamp();
        std::string formatted_message = "[" + timestamp + "] " + toString(status) + ": " + msg; 
        _log_file << formatted_message << std::endl;

        _log_file.close();  // this isn't pretty to close and open it so often but rn there is no other solution
    }
}

std::string debug::toString(debug::DEBUG_LEVEL level)
{
    switch (level)
    {
    case INFO: return "INFO";
    case ERROR: return "ERROR";
    case DB: return "DB";
    case GUI: return "GUI";
    case BACKEND: return "BACKEND";
    }
    return "You are f*cked";
}

void debug::print::debprint(const std::string &msg, debug::DEBUG_LEVEL level)
{
    #ifdef DEBUG
        std::cout << "[" << debug::toString(level) << "] " 
            << msg << "\n";
    #endif
    if (log_active)
    {
        log(msg, level);
    }
}

void debug::print::deberr(const std::string &err, debug::DEBUG_LEVEL level)
{
    std::cerr << "[" << debug::toString(ERROR) 
        << "/" << debug::toString(level) << "] " 
        << err << "\n";
    if (log_active)
    {
        log(err, level);
    }
}

void debug::print::fdebprint(const char *msg, debug::DEBUG_LEVEL level, ...)
{
    #ifdef DEBUG
        va_list args;
        va_start(args, msg);

        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), msg, args);

        va_end(args);
        debprint(std::string(buffer), level);
    #endif
    if (log_active)
    {
        log(msg, level);
    }
}

void debug::print::fdeberr(const char *err, debug::DEBUG_LEVEL level, ...)
{
    va_list args;
    va_start(args, err);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), err, args);

    va_end(args);
    deberr(std::string(buffer), level);
    if (log_active)
    {
        log(err, ERROR);
    }
}
