/**
 * @file cycle_sort.hpp
 * @brief  Class for cycle sort.
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

class CycleSort
{
    public:
        inline static const std::string name = "Cycle Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});
    
    private:
        CycleSort() = default;
};

template <typename T, typename Compare>
void CycleSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();

    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    auto same_key = [&](const T& a, const T& b) -> bool {
        return !cmp(a, b) && !cmp(b, a);
    };

    for (std::size_t i = 0; i < n - 1; ++i) {

        std::size_t index = i;
        T ele = arr[i];

        for (std::size_t j = i + 1; j < n; ++j) {
            if (cmp(arr[j], ele)) ++ index;
        }

        if (index == i) continue;

        while (/* index < n && */ same_key(ele, arr[index])) ++ index;
        std::swap(ele, arr[index]);

        while (index != i) {
            index = i;

            for (std::size_t j = i + 1; j < n; ++j) {
                if (cmp(arr[j], ele)) ++ index;
            }

            while (same_key(ele, arr[index])) ++ index;
            std::swap(ele, arr[index]);
        }
    }
}

} // namespace sort_algo
