/**
 * @file cpp_sorts.hpp
 * @brief  Wrapper classes for common C++ built-in sorting.
 * @author MingMinNa
 */

#pragma once

#include "../include/CppSortingAlgos/sort_utils.hpp"
#include <span>
#include <string>
#include <cstddef>
#include <utility>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace sort_algo 
{

// For std::sort
class StdSort 
{
    public:
        inline static const std::string name = "std::sort";
        inline static const bool is_stable   = false;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});

    private:
        StdSort() = default;
};

template <typename T, typename Compare>
void StdSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();

    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::sort(arr, arr + n, cmp);
}

// For std::stable_sort
class StdStableSort 
{
    public:
        inline static const std::string name = "std::stable_sort";
        inline static const bool is_stable   = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});

    private:
        StdStableSort() = default;
};

template <typename T, typename Compare>
void StdStableSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();

    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::stable_sort(arr, arr + n, cmp);
}

} // namespace sort_algo