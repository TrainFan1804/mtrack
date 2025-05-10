#include <algorithm>

#include "utils/str_manipulation.h"

std::string mtrack::strToLower(const std::string &str)
{
    std::string res = str;
    std::transform(
        res.begin(), res.end(), res.begin(),
        [](unsigned char c){
            return std::tolower(c);
        }
    );
    return res;
}
