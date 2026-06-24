/**
 * @file insertion_sort.hpp
 * @brief  Class for insertion sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
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
        InsertionSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return true; }
        static inline bool is_comparison()  { return true; }
        static inline bool in_place()       { return true; }
};

template <typename T, typename Compare>
void InsertionSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (size_t i = 1; i < n; ++i) {

        size_t index = i;
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
        BinaryInsertionSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return true; }
        static inline bool is_comparison()  { return true; }
        static inline bool in_place()       { return true; }
};

template <typename T, typename Compare>
void BinaryInsertionSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    for (size_t i = 1; i < n; ++i) {

        T ele = arr[i];
        size_t index = i;

        for (size_t jump = i; jump >= 1; jump >>= 1) {
            while (index >= jump && cmp(ele, arr[index - jump])) {
                index -= jump;
            }
        }

        for (size_t j = i; j > index; --j) {
            arr[j] = arr[j - 1];
        }

        arr[index] = ele;
    }
}

} // namespace sort_imp

