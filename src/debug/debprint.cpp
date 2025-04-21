#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <fstream>

#include "debug/debprint.h"
#include "globals.h"

namespace
{
    std::ofstream _log_file;

    std::string getStatusString(debug::DEBUG_LEVEL status)
    {
        switch (status) 
        {
        case debug::INFO: return "INFO";
        case debug::ERROR: return "ERROR";
        default: return "UNKNOWN";
        }
    }

    void log(const std::string &msg, debug::DEBUG_LEVEL status)
    {
        if (!log_active)
            return;

        _log_file.open(LOG_PATH_STR, std::ios::app);
        if (!_log_file.is_open()) throw std::runtime_error("Couldn't open log file");

        std::string timestamp = getCustomCurrentTimestamp();
        std::string formatted_message = "[" + timestamp + "] " + getStatusString(status) + ": " + msg; 
        _log_file << formatted_message << std::endl;

        _log_file.close();  // this isn't pretty to close and open it so often but rn there is no other solution
    }
}

void debug::print::debprint(const std::string &msg)
{
    #ifdef DEBUG
        std::cout << "[INFO] " << msg << "\n";
    #endif
    if (log_active)
    {
        log(msg, INFO);
    }
}

void debug::print::deberr(const std::string &err)
{
    #ifdef DEBUG
        std::cerr << "[ERROR] " << err << "\n";
    #endif
    if (log_active)
    {
        log(err, ERROR);
    }
}

void debug::print::fdebprint(const char *msg, ...)
{
    #ifdef DEBUG
        va_list args;
        va_start(args, msg);

        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), msg, args);

        va_end(args);
        debprint(std::string(buffer));
    #endif
    if (log_active)
    {
        log(msg, INFO);
    }
}

void debug::print::fdeberr(const char *err, ...)
{
    #ifdef DEBUG
        va_list args;
        va_start(args, err);

        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), err, args);

        va_end(args);
        deberr(std::string(buffer));
    #endif
    if (log_active)
    {
        log(err, ERROR);
    }
}
