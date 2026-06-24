/**
 * @file flash_test.cpp
 * @brief  Tests for flash sort.
 * @author MingMinNa
 */

#include "../include/flash_sort.hpp"
#include "test_datatypes.hpp"
#include "test_utils.hpp"
#include <vector>
#include <string>

using namespace sort_imp;
using namespace sort_test;

/* Global Variables */

std::vector<size_t> sort_test::test_sizes 
    = { 1u, 2u, 3u, 5u, 8u, 10u, 23u, 57u, 100u, 500u, 1000u, 5000u };

Int sort_test::Int_range_min = -1000;
Int sort_test::Int_range_max =  1000;

Float sort_test::Float_range_min = -100.0;
Float sort_test::Float_range_max =  100.0;

decltype(Element::key) sort_test::Element_range_min_key = 0;
decltype(Element::key) sort_test::Element_range_max_key = 2000;

using Sort = FlashSort;
std::string sort_name = "Flash Sort";

int main()
{
    bool test_int     = test<Sort, Int, IntCmp, IntFn>(sort_name);
    bool test_float   = test<Sort, Float, FloatCmp, FloatFn>(sort_name);
    bool test_element = test<Sort, Element, ElementCmp, ElementFn>(sort_name);
    return (test_int && test_float && test_element) ? (0) : (1);
}
