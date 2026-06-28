/**
 * @file bitonic_test.cpp
 * @brief  Tests for bitonic sort.
 * @author MingMinNa
 */

#include "../include/CppSortingAlgos/bitonic_sort.hpp"
#include "test_datatypes.hpp"
#include "test_utils.hpp"
#include <vector>
#include <string>

using namespace sort_imp;
using namespace sort_test;

/* Global Variables */

std::vector<std::size_t> sort_test::test_sizes 
    = { 1u, 2u, 4u, 8u, 16u, 128u, 512u, 2048u };

Int sort_test::Int_range_min = -10000;
Int sort_test::Int_range_max =  10000;

Float sort_test::Float_range_min = -100.0;
Float sort_test::Float_range_max =  100.0;

decltype(Element::key) sort_test::Element_range_min_key = 0;
decltype(Element::key) sort_test::Element_range_max_key = 20000;

using Sort = BitonicSort;
std::string sort_name = Sort::name;
Tests exclude = Tests::PartialRange;

int main()
{
    bool test_int     = test<Sort, Int, IntCmp, IntFn>(sort_name, exclude);
    bool test_float   = test<Sort, Float, FloatCmp, FloatFn>(sort_name, exclude);
    bool test_element = test<Sort, Element, ElementCmp, ElementFn>(sort_name, exclude);
    return (test_int && test_float && test_element) ? (0) : (1);
}
