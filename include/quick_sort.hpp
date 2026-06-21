/**
 * @file quick_sort.hpp
 * @brief  Class for quick sort.
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

class QuickSort
{
    public:
        static constexpr bool RANDOM_PIVOT_DEFAULT = true;

        QuickSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(
            T* arr, size_t n, 
            Compare cmp = Compare{}, 
            bool random_pivot = RANDOM_PIVOT_DEFAULT
        );
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }

        template <typename T, typename Compare>
        static size_t partition(
            T* arr, size_t start, size_t end, 
            Compare cmp, bool random_pivot
        );

    private:

        // Note: [start, end)
        template <typename T, typename Compare>
        static void quick_sort(
            T* arr, size_t start, size_t end, 
            Compare cmp, bool random_pivot
        );
};

template <typename T, typename Compare>
void QuickSort::sort(T* arr, size_t n, Compare cmp, bool random_pivot)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
    quick_sort(arr, 0, n, cmp, random_pivot);
}

template <typename T, typename Compare>
void QuickSort::quick_sort(
    T* arr, size_t start, size_t end, 
    Compare cmp, bool random_pivot
) {
    if (start + 1 >= end) return;

    size_t pivot_index = partition(arr, start, end, cmp, random_pivot);
    if (pivot_index > start)   quick_sort(arr, start, pivot_index, cmp, random_pivot);
    if (pivot_index + 1 < end) quick_sort(arr, pivot_index + 1, end, cmp, random_pivot);
}

template <typename T, typename Compare>
size_t QuickSort::partition(
    T* arr, size_t start, size_t end, 
    Compare cmp, bool random_pivot
) {
    size_t last = end - 1;

    if (random_pivot) {
        size_t rand_index = random_generate(start, last);
        std::swap(arr[last], arr[rand_index]);
    }

    T pivot = arr[last];
    size_t i = start;

    for (size_t j = start; j < last; ++j) {
        if (cmp(arr[j], pivot)) {
            std::swap(arr[i], arr[j]);
            ++ i;
        }
    }

    std::swap(arr[last], arr[i]);
    return i;
}

} // namespace sort_imp

