/**
 * @file shell_test.cpp
 * @brief  Tests for Shell sort.
 * @author MingMinNa
 */

#include "../include/shell_sort.hpp"
#include "test_datatypes.hpp"
#include "test_utils.hpp"
#include <string>

using namespace sort_imp;
using namespace sort_test;

using Sort = ShellSort;
std::string sort_name = "Shell Sort";

int main()
{
    bool test_int     = test<Sort, Int, IntCmp, IntFn>(sort_name);
    bool test_float   = test<Sort, Float, FloatCmp, FloatFn>(sort_name);
    bool test_element = test<Sort, Element, ElementCmp, ElementFn>(sort_name);
    return (test_int && test_float && test_element) ? (0) : (1);
}
