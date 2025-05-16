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

std::string mtrack::trim(const std::string &str)
{
    const char* WHITE_SPACE = " \t\v\r\n";
    std::size_t start = str.find_first_not_of(WHITE_SPACE);
    std::size_t end = str.find_last_not_of(WHITE_SPACE);
    return start == end ? std::string() : str.substr(start, end - start + 1);
}
