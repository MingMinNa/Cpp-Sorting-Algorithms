/**
 * @file selection_sort.hpp
 * @brief  Class for selection sort.
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

class SelectionSort
{
    public:
        inline static const std::string name = "Selection Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        SelectionSort() = default;
};

template <typename T, typename Compare>
void SelectionSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (std::size_t i = 1; i < n; ++i) {
        
        std::size_t index = i - 1;
        for (std::size_t j = i; j < n; ++j) {
            if (cmp(arr[j], arr[index])) {
                index = j;
            }
        }

        std::swap(arr[i - 1], arr[index]);
    }
}

} // namespace sort_imp

