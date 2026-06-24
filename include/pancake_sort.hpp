/**
 * @file pancake_sort.hpp
 * @brief  Class for pancake sort.
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

class PancakeSort
{
    public:
        PancakeSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }

    private:
    
        template <typename T>
        static void flip(T* arr, size_t l, size_t r);

        template <typename T, typename Compare>
        static size_t find_index(T* arr, size_t n, Compare cmp);
};

template <typename T, typename Compare>
void PancakeSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (size_t i = 0; i < n - 1; ++i) {    
        size_t index = find_index(arr, n - i, cmp);
        if (index == n - i - 1) continue;
        flip(arr, 0, index);
        flip(arr, 0, n - i - 1);
    }
}

template <typename T>
void PancakeSort::flip(T* arr, size_t l, size_t r)
{
    for (size_t i = l, j = r; i < j; ++i, --j) {
        std::swap(arr[i], arr[j]);
    }
}

template <typename T, typename Compare>
size_t PancakeSort::find_index(T* arr, size_t n, Compare cmp)
{
    size_t index = 0;
    for (size_t i = 0; i < n; ++i) {
        if (cmp(arr[index], arr[i])) index = i;
    }
    return index;
}

} // namespace sort_imp

