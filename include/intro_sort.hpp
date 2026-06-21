/**
 * @file intro_sort.hpp
 * @brief  Class for intro sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include "heap_sort.hpp"
#include "quick_sort.hpp"
#include <bit>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class IntroSort
{
    public:
        IntroSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }

    private:

        template <typename T, typename Compare>
        static void intro_sort_depth(T* arr, size_t start, size_t end, size_t depth, Compare cmp);
};

template <typename T, typename Compare>
void IntroSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    size_t max_depth = (std::bit_width(n) - 1) << 1;
    intro_sort_depth(arr, 0, n, max_depth, cmp);
}

template <typename T, typename Compare>
void IntroSort::intro_sort_depth(T* arr, size_t start, size_t end, size_t depth, Compare cmp) 
{    
    size_t n = end - start;

    if (n < 16) {
        InsertionSort::sort(arr + start, n, cmp);
        return;
    }
    else if (depth == 0) {
        HeapSort::sort(arr + start, n, cmp);
        return;
    }

    size_t pivot_index = QuickSort::partition(arr, start, end, cmp, true);
    if (pivot_index > start)   intro_sort_depth(arr, start, pivot_index, depth - 1, cmp);
    if (pivot_index + 1 < end) intro_sort_depth(arr, pivot_index + 1, end, depth - 1, cmp);
}

} // namespace sort_imp

