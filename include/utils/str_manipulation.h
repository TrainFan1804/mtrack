#ifndef STRMANIPULATION_H
#define STRMANIPULATION_H

#include <string>

namespace mtrack
{
    /*
        Why isn't there a function like this is std?
    */
    std::string strToLower(const std::string &str);

    /*
        Source because there isn't such easy thing in C++ by default:
        https://cplusplus.com/forum/beginner/251052/
     */
    std::string trim(const std::string &str);
}

#endif
