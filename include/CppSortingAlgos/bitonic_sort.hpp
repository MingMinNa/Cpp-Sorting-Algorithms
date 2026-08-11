/**
 * @file bitonic_sort.hpp
 * @brief  Class for bitonic sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class BitonicSort
{
    public:
        inline static const std::string name = "Bitonic Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        BitonicSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void bitonic_sort(T* arr, std::size_t n, Compare cmp, bool flag);

        template <typename T, typename Compare = std::less<T>>
        static void bitonic_merge(T* arr, std::size_t n, Compare cmp, bool flag);
};

template <typename T, typename Compare>
void BitonicSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    // assume the size of array is a power of 2.
    if ((n & (n - 1)) != 0) {
        throw std::runtime_error(
            "The size of array in bitonic sort must be a power of 2."
        );
    }

    bitonic_sort(arr, n, cmp, true);
}

template <typename T, typename Compare>
void BitonicSort::bitonic_sort(T* arr, std::size_t n, Compare cmp, bool flag)
{
    if (n > 1) {
        std::size_t mid = n >> 1;
        bitonic_sort(arr      ,     mid, cmp, true);
        bitonic_sort(arr + mid, n - mid, cmp, false);
        bitonic_merge(arr, n, cmp, flag);
    }
}

template <typename T, typename Compare>
void BitonicSort::bitonic_merge(T* arr, std::size_t n, Compare cmp, bool flag)
{
    if (n > 1) {
        std::size_t mid = n >> 1;
        for (std::size_t i = 0; i < mid; ++i) {
            if (
                ( flag && cmp(arr[i + mid], arr[i])) ||
                (!flag && cmp(arr[i], arr[i + mid]))
            ) std::swap(arr[i], arr[i + mid]);
        }
        
        bitonic_merge(arr      ,     mid, cmp, flag);
        bitonic_merge(arr + mid, n - mid, cmp, flag);
    }
}

} // namespace sort_algo
