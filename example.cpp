/**
 * @file example.cpp
 * @brief  Show how to use a sorting algorithm, using Bubble Sort as an example.
 * @author MingMinNa
 */

#include "include/CppSortingAlgos/bubble_sort.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <type_traits>

struct Element {
    int key;
    int value;
};

/* Comparators */
// note that the comparators must be either strictly increasing or strictly decreasing.
auto less_cmp = [](const Element& a, const Element& b) {
    return a.key < b.key;
};

auto greater_cmp = [](const Element& a, const Element& b) {
    return a.key > b.key;
};

/* Pretty print */
void print(const std::vector<Element>& v, const std::string& title)
{
    std::cout << title << "\n";
    for (const auto& e : v) {
        std::cout << "key = " << std::setw(2) << e.key
                  << ", value = " << e.value << "\n";
    }
    std::cout << "----------------------\n";
}

/* Trait Printer */
template <typename Sort>
void print_traits(const std::string& name)
{
    std::cout << "Traits of " << name << "\n";
    std::cout << "  is_stable      : " << (Sort::is_stable ? "true" : "false") << "\n";
    std::cout << "  is_comparison  : " << (Sort::is_comparison ? "true" : "false") << "\n";
    std::cout << "  in_place       : " << (Sort::in_place ? "true" : "false") << "\n";
    std::cout << "----------------------\n";
}

int main()
{
    using Sort = sort_algo::BubbleSort;

    std::vector<Element> data = {
        {70, 1},
        {30, 2},
        {90, 3},
        {10, 4},
        {70, 5},
        {50, 6},
        {10, 7}
    };

    print_traits<Sort>(Sort::name);
    print(data, "Original");

    Sort::sort(data.data(), data.size(), less_cmp);
    print(data, "Sorted (Ascending)");

    Sort::sort(data.data(), data.size(), greater_cmp);
    print(data, "Sorted (Descending)");

    return 0;
}

/* Output as below */
/*
    Traits of Bubble Sort
    is_stable      : true
    is_comparison  : true
    in_place       : true
    ----------------------
    Original
    key = 70, value = 1
    key = 30, value = 2
    key = 90, value = 3
    key = 10, value = 4
    key = 70, value = 5
    key = 50, value = 6
    key = 10, value = 7
    ----------------------
    Sorted (Ascending)
    key = 10, value = 4
    key = 10, value = 7
    key = 30, value = 2
    key = 50, value = 6
    key = 70, value = 1
    key = 70, value = 5
    key = 90, value = 3
    ----------------------
    Sorted (Descending)
    key = 90, value = 3
    key = 70, value = 1
    key = 70, value = 5
    key = 50, value = 6
    key = 30, value = 2
    key = 10, value = 4
    key = 10, value = 7
    ----------------------
*/