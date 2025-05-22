#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <fstream>

#include "external/fmt/format.h"

#include "debug/debprint.h"
#include "utils/time.h"
#include "globals/globals.h"

namespace
{
    bool _log_state;
    std::ofstream _log_file;

    void log(const std::string &msg, debug::DEBUG_LEVEL status)
    {
        if (!_log_file.is_open()) throw std::runtime_error("Couldn't open log file");

        std::string timestamp = mtrack::getCustomCurrentTimestamp("%d-%m-%Y %H-%M-%S");
        std::string formatted_message = "[" + timestamp + "] " + toString(status) + ": " + msg; 
        _log_file << formatted_message << std::endl;
    }

    void init()
    {
        auto date = mtrack::getCustomCurrentTimestamp("%Y-%m-%d");
        std::string LOG_PATH = LOG_BASE_PATH + "/" + date + ".log";
        _log_file.open(LOG_PATH, std::ios::app);
    }
    
    void kill()
    {
        _log_file.close();
    }
}

void debug::setState(bool state)
{
    _log_state = state;
    if (_log_state)
    {
        init();
    }
    else
    {
        kill();
    }
}

std::string debug::toString(DEBUG_LEVEL level)
{
    switch (level)
    {
    case ERROR: return "ERROR";
    case WARNING: return "WARNING";
    case INFO: return "INFO";
    }
    return "You are f*cked";
}

void debug::print::debprint(const std::string &msg, DEBUG_LEVEL level)
{
    if (_log_state)
    {
        log(msg, level);
    }
    if (level == ERROR)
    {
        std::cerr << "[" << debug::toString(level) << "] " 
            << msg << "\n";
        return;
    }
    #ifdef DEBUG
        std::cout << "[" << debug::toString(level) << "] " 
            << msg << "\n";
    #endif
}
