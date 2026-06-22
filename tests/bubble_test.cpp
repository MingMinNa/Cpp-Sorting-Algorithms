/**
 * @file bubble_test.cpp
 * @brief  Tests for bubble sort.
 * @author MingMinNa
 */

#include "../include/bubble_sort.hpp"
#include "test_datatypes.hpp"
#include "test_utils.hpp"
#include <string>

using namespace sort_imp;
using namespace sort_test;

using Sort = BubbleSort;
std::string sort_name = "Bubble Sort";

int main()
{
    bool test_int     = test<Sort, Int, IntCmp, IntFn>(sort_name);
    bool test_float   = test<Sort, Float, FloatCmp, FloatFn>(sort_name);
    bool test_element = test<Sort, Element, ElementCmp, ElementFn>(sort_name);
    return (test_int && test_float && test_element) ? (0) : (1);
}
