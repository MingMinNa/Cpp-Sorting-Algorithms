/**
 * @file counting_test.cpp
 * @brief  Tests for counting sort.
 * @author MingMinNa
 */

#include "../include/counting_sort.hpp"
#include "test_datatypes.hpp"
#include "test_utils.hpp"
#include <string>

using namespace sort_imp;
using namespace sort_test;

using Sort = CountingSort;
std::string sort_name = "Counting Sort";

int main()
{
    bool test_int     = test<Sort, Int, IntCmp, IntFn>(sort_name);
    bool test_element = test<Sort, Element, ElementCmp, ElementFn>(sort_name);
    return (test_int && test_element) ? (0) : (1);
}
