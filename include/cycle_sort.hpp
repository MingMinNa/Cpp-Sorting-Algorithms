/**
 * @file cycle_sort.hpp
 * @brief  Class for cycle sort.
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

class CycleSort
{
    public:
        CycleSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return true;  }
};

template <typename T, typename Compare>
void CycleSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    auto same_key = [&](const T& a, const T& b) -> bool {
        return !cmp(a, b) && !cmp(b, a);
    };

    for (size_t i = 0; i < n - 1; ++i) {

        size_t index = i;
        T ele = arr[i];

        for (size_t j = i + 1; j < n; ++j) {
            if (cmp(arr[j], ele)) ++ index;
        }

        if (index == i) continue;

        while (/* index < n && */ same_key(ele, arr[index])) ++ index;
        std::swap(ele, arr[index]);

        while (index != i) {
            index = i;

            for (size_t j = i + 1; j < n; ++j) {
                if (cmp(arr[j], ele)) ++ index;
            }

            while (same_key(ele, arr[index])) ++ index;
            std::swap(ele, arr[index]);
        }
    }
}

} // namespace sort_imp
