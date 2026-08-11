/**
 * @file intro_sort.hpp
 * @brief  Class for intro sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include "heap_sort.hpp"
#include "quick_sort.hpp"
#include <bit>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class IntroSort
{
    public:
        inline static const std::string name = "Intro Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        IntroSort() = default;

        template <typename T, typename Compare>
        static void intro_sort_depth(
            T* arr, std::size_t start, std::size_t end, 
            std::size_t depth, Compare cmp
        );
};

template <typename T, typename Compare>
void IntroSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t max_depth = (std::bit_width(n) - 1) << 1;
    intro_sort_depth(arr, 0, n, max_depth, cmp);
}

template <typename T, typename Compare>
void IntroSort::intro_sort_depth(
    T* arr, std::size_t start, std::size_t end, 
    std::size_t depth, Compare cmp
) {    
    std::size_t n = end - start;

    if (n < 16) {
        InsertionSort::sort(arr + start, n, cmp);
        return;
    }
    else if (depth == 0) {
        HeapSort::sort(arr + start, n, cmp);
        return;
    }

    std::size_t pivot_index = QuickSort::partition(arr, start, end, cmp, true);
    if (pivot_index > start)   intro_sort_depth(arr, start, pivot_index, depth - 1, cmp);
    if (pivot_index + 1 < end) intro_sort_depth(arr, pivot_index + 1, end, depth - 1, cmp);
}

} // namespace sort_algo

