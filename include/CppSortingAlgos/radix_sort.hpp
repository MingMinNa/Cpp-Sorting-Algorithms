/**
 * @file radix_sort.hpp
 * @brief  Class for radix sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <string>
#include <cstddef>
#include <utility>
#include <cstring>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class RadixSort
{
    public:
        inline static const std::string name = "Radix Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = false;
        inline static const bool in_place      = false;

        inline static const std::size_t RADIX = 1u << 8;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});
    
    private:
        RadixSort() = default;
};

template <typename T, typename Compare>
void RadixSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    // Note: radix must be at least 2.
    // General Case: std::size_t radix = ..., div = 1;
    std::size_t radix = RADIX, pow = 0;
    auto [min_ele, max_ele] = find_min_max(arr, n);
    bool descending = cmp(max_ele, min_ele);
    std::vector<T> temp_arr(arr, arr + n);

    // General Case: max_ele >= div
    while ((max_ele >> pow) > 0) {
        
        std::copy(arr, arr + n, temp_arr.data());

        // Counting Sort
        std::vector<T> counts(radix);

        for (std::size_t i = 0; i < n; ++i) {
            // General Case: std::size_t radix_index = (temp_arr[i] / div) % radix;
            std::size_t radix_index = (temp_arr[i] >> pow) & (radix - 1);
            counts[radix_index] ++;
        }

        ptrdiff_t start = 0, end = 0, step = 0;

        if (!descending) start = 1        , end = radix, step =  1;
        else             start = radix - 2, end = -1   , step = -1;

        for (ptrdiff_t i = start; i != end; i += step) {
            counts[i] += counts[i - step];
        }

        for (ptrdiff_t i = n - 1; i >= 0; --i) {
            // General Case: std::size_t radix_index = (temp_arr[i] / div) % radix;
            std::size_t radix_index = (temp_arr[i] >> pow) & (radix - 1);
            std::size_t rank = static_cast<std::size_t>(counts[radix_index]);

            // rank is 1-based index.
            arr[TO_ZERO_BASE(rank)] = temp_arr[i];
            counts[radix_index] --;
        }

        // General Case: div *= radix;
        pow += 8; 
    }
}

} // namespace sort_algo

