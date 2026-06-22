/**
 * @file radix_test.cpp
 * @brief  Tests for radix sort.
 * @author MingMinNa
 */

#include "../include/radix_sort.hpp"
#include "test_datatypes.hpp"
#include "test_utils.hpp"
#include <string>

using namespace sort_imp;
using namespace sort_test;

using Sort = RadixSort;
std::string sort_name = "Radix Sort";

int main()
{
    bool test_element = test<Sort, Element, ElementCmp, ElementFn>(sort_name);
    return (test_element) ? (0) : (1);
}
