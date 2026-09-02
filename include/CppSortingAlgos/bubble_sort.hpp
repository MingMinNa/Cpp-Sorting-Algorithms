/**
 * @file bubble_sort.hpp
 * @brief  Class for bubble sort.
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

class BubbleSort
{
    public:
        inline static const std::string name = "Bubble Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});

    private:
        BubbleSort() = default;
};

template <typename T, typename Compare>
void BubbleSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();
    
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (std::size_t i = 1; i < n; ++i) {

        bool swapped = false;
        for (std::size_t j = 0; j < n - i; ++j) {
            if (cmp(arr[j + 1], arr[j])) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        // The bubble sort is complete
        if (!swapped) break;
    }
}

} // namespace sort_algo

