/**
 * @file heap_sort.hpp
 * @brief  Class for heap sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <cstddef>
#include <functional>
#include <stdexcept>

namespace sort_imp 
{

class HeapSort
{
    public:
        HeapSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }

    private:
        template <typename T, typename Compare>
        static void build_heap(T* arr, size_t n, Compare cmp);

        template <typename T, typename Compare>
        static void heapify(T* arr, size_t n, size_t curr_index, Compare cmp);
};

template <typename T, typename Compare>
void HeapSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    build_heap<T, Compare>(arr, n, cmp);

    for (size_t i = n; i > 0; --i) {
        std::swap(arr[TO_ZERO_BASE(i)], arr[0]);
        heapify(arr, i - 1, 1, cmp);
    }
}

template <typename T, typename Compare>
void HeapSort::build_heap(T* arr, size_t n, Compare cmp)
{
    for (size_t i = (n >> 1) + 1; i > 0; --i) {
        heapify(arr, n, i, cmp);
    }
}

template <typename T, typename Compare>
void HeapSort::heapify(T* arr, size_t n, size_t curr_index, Compare cmp)
{
    while (true) {
        size_t left  = curr_index << 1;
        size_t right = left + 1;
        size_t next_index = curr_index;

        // Note: left and right are 1-based index.
        if (left  <= n && cmp(arr[TO_ZERO_BASE(next_index)], arr[TO_ZERO_BASE(left)] )) next_index = left;
        if (right <= n && cmp(arr[TO_ZERO_BASE(next_index)], arr[TO_ZERO_BASE(right)])) next_index = right;

        if (next_index == curr_index) return;

        std::swap(arr[TO_ZERO_BASE(curr_index)], arr[TO_ZERO_BASE(next_index)]);
        curr_index = next_index;
    }
}

} // namespace sort_imp