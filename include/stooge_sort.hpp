/**
 * @file stooge_sort.hpp
 * @brief  Class for stooge sort.
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

class StoogeSort
{
    public:
        StoogeSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }

    private:

        // Note: [start, end)
        template <typename T, typename Compare>
        static void stooge_sort(T* arr, size_t start, size_t end, Compare cmp);
};

template <typename T, typename Compare>
void StoogeSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
    stooge_sort(arr, 0, n, cmp);
}

template <typename T, typename Compare>
void StoogeSort::stooge_sort(T* arr, size_t start, size_t end, Compare cmp)
{
    if (start + 1 >= end) return;

    if (cmp(arr[end - 1], arr[start])) 
        std::swap(arr[start], arr[end - 1]);

    if (end - start >= 3) {
        size_t offset = (end - start) / 3;
        stooge_sort(arr, start, end - offset, cmp);
        stooge_sort(arr, start + offset, end, cmp);
        stooge_sort(arr, start, end - offset, cmp);
    }
}

} // namespace sort_imp

