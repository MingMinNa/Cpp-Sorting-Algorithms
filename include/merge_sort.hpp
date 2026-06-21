/**
 * @file merge_sort.hpp
 * @brief  Class for merge sort.
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

class MergeSort
{
    public:
        static constexpr bool RANDOM_PIVOT_DEFAULT = true;

        MergeSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return true;  }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return false; }

    private:

        // Note: [start, end)
        template <typename T, typename Compare>
        static void merge_sort(T* arr, size_t start, size_t end, Compare cmp);

        template <typename T, typename Compare>
        static void merge(T* arr, size_t start, size_t mid, size_t end, Compare cmp);

};

template <typename T, typename Compare>
void MergeSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
    merge_sort(arr, 0, n, cmp);
}

template <typename T, typename Compare>
void MergeSort::merge_sort(T* arr, size_t start, size_t end, Compare cmp)
{
    if (start + 1 >= end) return;

    size_t mid = (start + end) >> 1;
    merge_sort(arr, start, mid, cmp);
    merge_sort(arr, mid, end, cmp);
    merge(arr, start, mid, end, cmp);
}

template <typename T, typename Compare>
void MergeSort::merge(T* arr, size_t start, size_t mid, size_t end, Compare cmp)
{
    size_t n1 = mid - start;
    size_t n2 = end - mid;
    
    std::vector<T> l_vec(arr + start, arr + mid);
    std::vector<T> r_vec(arr + mid, arr + end);
    size_t i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if (cmp(r_vec[j], l_vec[i])) arr[k ++] = r_vec[j ++];
        else                         arr[k ++] = l_vec[i ++];
    }

    while (i < n1) arr[k ++] = l_vec[i ++];
    while (j < n2) arr[k ++] = r_vec[j ++];
}

} // namespace sort_imp

