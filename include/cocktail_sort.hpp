/**
 * @file cocktail_sort.hpp
 * @brief  Class for cocktail sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <string>
#include <cstddef>
#include <functional>
#include <stdexcept>

namespace sort_imp 
{

class CocktailSort
{
    public:
        inline static const std::string name = "Cocktail Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        CocktailSort() = default;
};

template <typename T, typename Compare>
void CocktailSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t left = 0, right = n - 1;
    bool swapped = true;

    while (swapped && left < right) {
        
        swapped = false;
        for (std::size_t i = left; i < right; ++i) {
            if (cmp(arr[i + 1], arr[i])) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        right --;

        if (!swapped) break;

        for (std::size_t i = right; i > left; --i) {
            if (cmp(arr[i], arr[i - 1])) {
                std::swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
        left ++;
    }
}

} // namespace sort_imp