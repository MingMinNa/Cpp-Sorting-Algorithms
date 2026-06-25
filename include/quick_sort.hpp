/**
 * @file quick_sort.hpp
 * @brief  Class for quick sort.
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

class QuickSort
{
    public:
        inline static const std::string name = "Quick Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        inline static const bool RANDOM_PIVOT_DEFAULT = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(
            T* arr, std::size_t n, 
            Compare cmp = Compare{}, 
            bool random_pivot = RANDOM_PIVOT_DEFAULT
        );

        template <typename T, typename Compare>
        static std::size_t partition(
            T* arr, std::size_t start, std::size_t end, 
            Compare cmp, bool random_pivot
        );

    private:
        QuickSort() = default;

        // Note: [start, end)
        template <typename T, typename Compare>
        static void quick_sort(
            T* arr, std::size_t start, std::size_t end, 
            Compare cmp, bool random_pivot
        );
};

template <typename T, typename Compare>
void QuickSort::sort(T* arr, std::size_t n, Compare cmp, bool random_pivot)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
    quick_sort(arr, 0, n, cmp, random_pivot);
}

template <typename T, typename Compare>
void QuickSort::quick_sort(
    T* arr, std::size_t start, std::size_t end, 
    Compare cmp, bool random_pivot
) {
    if (start + 1 >= end) return;

    std::size_t pivot_index = partition(arr, start, end, cmp, random_pivot);
    if (pivot_index > start)   quick_sort(arr, start, pivot_index, cmp, random_pivot);
    if (pivot_index + 1 < end) quick_sort(arr, pivot_index + 1, end, cmp, random_pivot);
}

template <typename T, typename Compare>
std::size_t QuickSort::partition(
    T* arr, std::size_t start, std::size_t end, 
    Compare cmp, bool random_pivot
) {
    std::size_t last = end - 1;

    if (random_pivot) {
        std::size_t rand_index = random_generate(start, last);
        std::swap(arr[last], arr[rand_index]);
    }

    T pivot = arr[last];
    std::size_t i = start;

    for (std::size_t j = start; j < last; ++j) {
        if (cmp(arr[j], pivot)) {
            std::swap(arr[i], arr[j]);
            ++ i;
        }
    }

    std::swap(arr[last], arr[i]);
    return i;
}

} // namespace sort_imp

