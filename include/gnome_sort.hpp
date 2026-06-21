/**
 * @file gnome_sort.hpp
 * @brief  Class for gnome sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <cstddef>
#include <functional>
#include <stdexcept>

namespace sort_imp 
{

class GnomeSort
{
    public:
        GnomeSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return true; }
        static inline bool is_comparison()  { return true; }
        static inline bool in_place()       { return true; }
};

template <typename T, typename Compare>
void GnomeSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    size_t pos = 0;
    while (pos < n) {
        if (pos > 0 && cmp(arr[pos], arr[pos - 1])) {
            std::swap(arr[pos - 1], arr[pos]);
            -- pos;
        }
        else {
            ++ pos;
        }
    }
}

} // namespace sort_imp