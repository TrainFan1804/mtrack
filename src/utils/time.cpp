#include <ctime>
#include <iomanip>
#include <sstream>

#include "utils/time.h"

/**
 * Will return the current timestamp as a string.
 * 
 * @param format    The format the timestamp should be returned. E.g. 
 * `@code "%m-%d"` will return the current month and day.
 */
std::string mtrack::getCustomCurrentTimestamp(const std::string &format)
{
    auto time_now = std::time(nullptr);
    auto tm = *std::localtime(&time_now);

    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}
