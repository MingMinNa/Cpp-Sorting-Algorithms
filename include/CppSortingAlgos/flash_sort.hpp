/**
 * @file flash_sort.hpp
 * @brief  Class for flash sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include <span>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class FlashSort
{
    public:
        inline static const std::string name = "Flash Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = false;
        inline static const bool in_place      = false;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});
    
    private:
        FlashSort() = default;
};

template <typename T, typename Compare>
void FlashSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();
    
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t num_classes = std::max(
        static_cast<std::size_t>(n * 0.43), 
        static_cast<std::size_t>(2)
    );
    std::vector<std::size_t> classes(num_classes, 0);

    auto [min_ele, max_ele] = find_min_max(arr, n);
    bool descending = cmp(max_ele, min_ele);

    auto calc_index = [&](const T &ele) -> std::size_t {
        std::size_t index = static_cast<std::size_t>(
            (num_classes - 1) * (static_cast<double>(ele - min_ele)) / 
                                 static_cast<double>(max_ele - min_ele)
        );
        index = (index >= num_classes) ? (num_classes - 1) : (index);
        return (descending) ? (num_classes - 1 - index) : (index);
    };

    for (std::size_t i = 0; i < n; ++i) {
        std::size_t index = calc_index(arr[i]);
        classes[index] ++;
    }

    for (std::size_t i = 1; i < num_classes; ++i) {
        classes[i] += classes[i - 1];
    }

    std::size_t count = 0;
    std::size_t i = 0;
 
    while (count < n) {

        for (;i < n && i >= classes[calc_index(arr[i])]; ++i);
        if (i >= n) break;

        T ele = arr[i];
        std::size_t index = calc_index(ele);
 
        while (i != classes[index]) {
            index = calc_index(ele);
            -- classes[index]; 
            std::swap(ele, arr[classes[index]]); 
            ++ count;
        }
    }

    InsertionSort::sort(arr_span, cmp);
}

} // namespace sort_algo

