/**
 * @file slow_sort.hpp
 * @brief  Class for slow sort.
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

class SlowSort
{
    public:
        SlowSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }

    private:

        // Note: [start, end)
        template <typename T, typename Compare>
        static void slow_sort(T* arr, size_t start, size_t end, Compare cmp);
};

template <typename T, typename Compare>
void SlowSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
    slow_sort(arr, 0, n, cmp);
}

template <typename T, typename Compare>
void SlowSort::slow_sort(T* arr, size_t start, size_t end, Compare cmp)
{
    if (start + 1 >= end) return;

    size_t mid = (start + end) >> 1;
    slow_sort(arr, start, mid, cmp);
    slow_sort(arr, mid, end, cmp);

    if (cmp(arr[end - 1], arr[mid - 1])) 
        std::swap(arr[mid - 1], arr[end - 1]);

    slow_sort(arr, start, end - 1, cmp);
}

} // namespace sort_imp

