/**
 * @file selection_sort.hpp
 * @brief  Class for selection sort.
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

class SelectionSort
{
    public:
        SelectionSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }
};

template <typename T, typename Compare>
void SelectionSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (size_t i = 1; i < n; ++i) {
        
        size_t index = i - 1;
        for (size_t j = i; j < n; ++j) {
            if (cmp(arr[j], arr[index])) {
                index = j;
            }
        }

        std::swap(arr[i - 1], arr[index]);
    }
}

} // namespace sort_imp

