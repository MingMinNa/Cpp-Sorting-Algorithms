/**
 * @file comb_sort.hpp
 * @brief  Class for comb sort.
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

class CombSort
{
    public:
        inline static const std::string name = "Comb Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        inline static const double SHRINK_FACTOR = 0.769; //  1 / 1.3 ≈ 0.769

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        CombSort() = default;
};

template <typename T, typename Compare>
void CombSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t gap = n;
    bool swapped = true;

    while (gap > 1 || swapped) {
        
        gap = static_cast<std::size_t>(gap * SHRINK_FACTOR);

        if      (gap < 1)               gap = 1;
        else if (gap == 9 || gap == 10) gap = 11;

        swapped = false;

        for (std::size_t i = 0; i + gap < n; ++i) {
            if (cmp(arr[i + gap], arr[i])) {
                std::swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}

} // namespace sort_algo
