/**
 * @file pancake_sort.hpp
 * @brief  Class for pancake sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class PancakeSort
{
    public:
        inline static const std::string name = "Pancake Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        PancakeSort() = default;
    
        template <typename T>
        static void flip(T* arr, std::size_t l, std::size_t r);

        template <typename T, typename Compare>
        static std::size_t find_index(T* arr, std::size_t n, Compare cmp);
};

template <typename T, typename Compare>
void PancakeSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (std::size_t i = 0; i < n - 1; ++i) {    
        std::size_t index = find_index(arr, n - i, cmp);
        if (index == n - i - 1) continue;
        flip(arr, 0, index);
        flip(arr, 0, n - i - 1);
    }
}

template <typename T>
void PancakeSort::flip(T* arr, std::size_t l, std::size_t r)
{
    for (std::size_t i = l, j = r; i < j; ++i, --j) {
        std::swap(arr[i], arr[j]);
    }
}

template <typename T, typename Compare>
std::size_t PancakeSort::find_index(T* arr, std::size_t n, Compare cmp)
{
    std::size_t index = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (cmp(arr[index], arr[i])) index = i;
    }
    return index;
}

} // namespace sort_imp

