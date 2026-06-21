/**
 * @file radix_sort.hpp
 * @brief  Class for radix sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <cstddef>
#include <utility>
#include <cstring>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class RadixSort
{
    public:
        inline static const size_t RADIX = 1u << 8;
        RadixSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return true;  }
        static inline bool is_comparison()  { return false; }
        static inline bool in_place()       { return false; }
};

template <typename T, typename Compare>
void RadixSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    // Note: radix must be at least 2.
    // General Case: size_t radix = ..., div = 1;
    size_t radix = RADIX, pow = 0;
    auto [min_ele, max_ele] = find_min_max(arr, n);
    bool descending = cmp(max_ele, min_ele);
    std::vector<T> temp_arr(arr, arr + n);

    // General Case: max_ele >= div
    while ((max_ele >> pow) > 0) {
        
        memcpy(temp_arr.data(), arr, n * sizeof(T));

        // Radix sort
        std::vector<T> counts(radix);

        for (size_t i = 0; i < n; ++i) {
            // General Case: size_t radix_index = (temp_arr[i] / div) % radix;
            size_t radix_index = (temp_arr[i] >> pow) & (radix - 1);
            counts[radix_index] ++;
        }

        ptrdiff_t start = 0, end = 0, step = 0;

        if (!descending) start = 1        , end = radix, step =  1;
        else             start = radix - 2, end = -1   , step = -1;

        for (ptrdiff_t i = start; i != end; i += step) {
            counts[i] += counts[i - step];
        }

        for (ptrdiff_t i = n - 1; i >= 0; --i) {
            // General Case: size_t radix_index = (temp_arr[i] / div) % radix;
            size_t radix_index = (temp_arr[i] >> pow) & (radix - 1);
            size_t rank = static_cast<size_t>(counts[radix_index]);

            // rank is 1-based index.
            arr[TO_ZERO_BASE(rank)] = temp_arr[i];
            counts[radix_index] --;
        }

        // General Case: div *= radix;
        pow += 8; 
    }
}

} // namespace sort_imp

