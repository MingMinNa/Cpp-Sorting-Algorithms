/**
 * @file odd_even_sort.hpp
 * @brief  Class for odd-even sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class OddEvenSort
{
    public:
        inline static const std::string name = "Odd-Even Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});
    
    private:
        OddEvenSort() = default;
};

template <typename T, typename Compare>
void OddEvenSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    bool swapped = true;

    while (swapped) {
        swapped = false;
        for (std::size_t odd_even = 0; odd_even < 2; ++ odd_even) {
            for (std::size_t i = odd_even; i < n - 1; i += 2) {
                if (cmp(arr[i + 1], arr[i])) {
					std::swap(arr[i], arr[i + 1]);
					swapped = true;
				}
            }
        }
    }
}

} // namespace sort_imp

