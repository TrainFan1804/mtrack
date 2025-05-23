#include <cassert>
#include <iostream>

#include "utils/str_manipulation.h"

void test_trim()
{
    const std::string EXPECTED = "Hello World!";
    std::string l_tr_obj = "   Hello World!";
    std::string r_tr_obj = "Hello World!    ";
    std::string lr_tr_obj = "   Hello World!    ";

    mtrack::lTrim(l_tr_obj);
    assert(l_tr_obj == EXPECTED);
    assert(mtrack::rTrim(r_tr_obj) == EXPECTED);
    assert(mtrack::trim(lr_tr_obj) == EXPECTED);
}

int main()
{
    test_trim();
    std::cout << "All test passed!\n";
    return 0;
}
