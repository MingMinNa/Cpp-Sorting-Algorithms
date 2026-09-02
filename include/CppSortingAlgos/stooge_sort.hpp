/**
 * @file stooge_sort.hpp
 * @brief  Class for stooge sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <span>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class StoogeSort
{
    public:
        inline static const std::string name = "Stooge Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});

    private:
        StoogeSort() = default;

        // Note: [start, end)
        template <typename T, typename Compare>
        static void stooge_sort(T* arr, std::size_t start, std::size_t end, Compare cmp);
};

template <typename T, typename Compare>
void StoogeSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();
    
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
    stooge_sort(arr, 0, n, cmp);
}

template <typename T, typename Compare>
void StoogeSort::stooge_sort(T* arr, std::size_t start, std::size_t end, Compare cmp)
{
    if (start + 1 >= end) return;

    if (cmp(arr[end - 1], arr[start])) 
        std::swap(arr[start], arr[end - 1]);

    if (end - start >= 3) {
        std::size_t offset = (end - start) / 3;
        stooge_sort(arr, start, end - offset, cmp);
        stooge_sort(arr, start + offset, end, cmp);
        stooge_sort(arr, start, end - offset, cmp);
    }
}

} // namespace sort_algo

