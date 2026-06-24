/**
 * @file odd_even_sort.hpp
 * @brief  Class for odd-even sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class OddEvenSort
{
    public:
        OddEvenSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return true; }
        static inline bool is_comparison()  { return true; }
        static inline bool in_place()       { return true; }
};

template <typename T, typename Compare>
void OddEvenSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    bool swapped = true;

    while (swapped) {
        swapped = false;
        for (size_t odd_even = 0; odd_even < 2; ++ odd_even) {
            for (size_t i = odd_even; i < n - 1; i += 2) {
                if (cmp(arr[i + 1], arr[i])) {
					std::swap(arr[i], arr[i + 1]);
					swapped = true;
				}
            }
        }
    }
}

} // namespace sort_imp

