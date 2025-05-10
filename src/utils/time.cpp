#include <ctime>
#include <iomanip>
#include <sstream>

#include "utils/time.h"

std::string mtrack::getCustomCurrentTimestamp() 
{
    auto time_now = std::time(nullptr);
    auto tm = *std::localtime(&time_now);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    return oss.str();
}
