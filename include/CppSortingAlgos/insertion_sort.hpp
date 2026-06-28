/**
 * @file insertion_sort.hpp
 * @brief  Class for insertion sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

/* ————————————— Insertion Sort ————————————— */

namespace sort_imp 
{

class InsertionSort
{
    public:
        inline static const std::string name = "Insertion Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});
        
    private:
        InsertionSort() = default;
};

template <typename T, typename Compare>
void InsertionSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (std::size_t i = 1; i < n; ++i) {

        std::size_t index = i;
        T ele = arr[i];

        while (index > 0 && cmp(ele, arr[index - 1])) {
            arr[index] = arr[index - 1];
            -- index;
        }

        arr[index] = ele;
    }
}

} // namespace sort_imp

/* ————————————— Binary Insertion Sort ————————————— */

namespace sort_imp 
{

class BinaryInsertionSort
{
    public:
        inline static const std::string name = "Binary Insertion Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        BinaryInsertionSort() = default;
};

template <typename T, typename Compare>
void BinaryInsertionSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (std::size_t i = 1; i < n; ++i) {

        std::size_t index = i;
        T ele = arr[i];

        for (std::size_t jump = i; jump >= 1; jump >>= 1) {
            while (index >= jump && cmp(ele, arr[index - jump])) {
                index -= jump;
            }
        }

        for (std::size_t j = i; j > index; --j) {
            arr[j] = arr[j - 1];
        }

        arr[index] = ele;
    }
}

} // namespace sort_imp

