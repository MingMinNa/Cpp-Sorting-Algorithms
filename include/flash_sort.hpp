/**
 * @file flash_sort.hpp
 * @brief  Class for flash sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class FlashSort
{
    public:
        FlashSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return false; }
        static inline bool in_place()       { return false; }
};

template <typename T, typename Compare>
void FlashSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    size_t num_classes = std::max(static_cast<size_t>(n * 0.43), static_cast<size_t>(2));
    std::vector<size_t> classes(num_classes, 0);

    auto [min_ele, max_ele] = find_min_max(arr, n);
    bool descending = cmp(max_ele, min_ele);

    auto calc_index = [&](const T &ele) -> size_t {
        size_t index = static_cast<size_t>(
            (num_classes - 1) * (static_cast<size_t>(    ele - min_ele)) / 
                                 static_cast<double>(max_ele - min_ele)
        );
        index = (index >= num_classes) ? (num_classes - 1) : (index);
        return (descending) ? (num_classes - 1 - index) : (index);
    };

    for (size_t i = 0; i < n; ++i) {
        size_t index = calc_index(arr[i]);
        classes[index] ++;
    }

    for (size_t i = 1; i < num_classes; ++i) {
        classes[i] += classes[i - 1];
    }

    size_t count = 0;
    size_t i     = 0;
 
    while (count < n) {

        for (;i < n && i >= classes[calc_index(arr[i])]; ++i);
        if (i >= n) break;

        T ele = arr[i];
        size_t index = calc_index(ele);
 
        while (i != classes[index]) {
            index = calc_index(ele);
            -- classes[index]; 
            std::swap(ele, arr[classes[index]]); 
            ++ count;
        }
    }

    InsertionSort::sort(arr, n, cmp);
}

} // namespace sort_imp

