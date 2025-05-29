#include "utils/str_manipulation.h"

#include <algorithm>
#include <functional>

std::string mtrack::strToLower(const std::string &str)
{
    std::string res = str;
    std::transform(
        res.begin(), res.end(), res.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    return res;
}

// trim from start
std::string &mtrack::lTrim(std::string &str)
{
    // str.erase(str.begin(), std::find_if(str.begin(), str.end(),
    //             std::ptr_fun<int, int>(std::isspace)));

    str.erase(
        str.begin(), std::find_if(
                         str.begin(), str.end(),
                         [](unsigned char c) { return !std::isspace(c); }
                     )
    );
    return str;
}

// trim from end
std::string &mtrack::rTrim(std::string &str)
{
    // str.erase(std::find_if(str.rbegin(), str.rend(),
    //         std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
    //         str.end());

    str.erase(
        std::find_if(
            str.rbegin(), str.rend(),
            [](unsigned char c) { return !std::isspace(c); }
        ).base(),
        str.end()
    );
    return str;
}

std::string mtrack::trim(const std::string &str)
{
    std::string copy = str;
    return lTrim(rTrim(copy));
}
