/**
 * @file gnome_sort.hpp
 * @brief  Class for gnome sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <span>
#include <string>
#include <cstddef>
#include <functional>
#include <stdexcept>

namespace sort_algo 
{

class GnomeSort
{
    public:
        inline static const std::string name = "Gnome Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});

    private:
        GnomeSort() = default;
};

template <typename T, typename Compare>
void GnomeSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();
    
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t index = 0;
    while (index < n) {
        if (index > 0 && cmp(arr[index], arr[index - 1])) {
            std::swap(arr[index - 1], arr[index]);
            -- index;
        }
        else {
            ++ index;
        }
    }
}

} // namespace sort_algo