/**
 * @file counting_sort.hpp
 * @brief  Class for counting sort.
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

class CountingSort
{
    public:
        inline static const std::string name = "Counting Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = false;
        inline static const bool in_place      = false;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});
    
    private:
        CountingSort() = default;
};

template <typename T, typename Compare>
void CountingSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    auto [min_ele, max_ele] = find_min_max(arr, n);
    std::size_t size = static_cast<std::size_t>(max_ele - min_ele + 1);

    // overflow (e.g. max_ele == LLONG_MAX, min_ele == LLONG_MIN, max_ele - min_ele + 1 will be 0)
    if (size == 0) {
        throw std::runtime_error(
            "Range computation overflow (max - min + 1)"
        );
    }

    std::vector<std::size_t> counts(size);

    for (std::size_t i = 0; i < n; ++i) {
        counts[arr[i] - min_ele] ++;
    }

    bool descending = cmp(max_ele, min_ele);
    std::vector<T> temp_arr(arr, arr + n);
    ptrdiff_t start = 0, end = 0, step = 0;

    if (!descending) start = 1       , end = size, step =  1;
    else             start = size - 2, end = -1  , step = -1;

    for (ptrdiff_t i = start; i != end; i += step) {
        counts[i] += counts[i - step];
    }

    for (ptrdiff_t i = n - 1; i >= 0; --i) {
        std::size_t rank = counts[temp_arr[i] - min_ele];
        
        // rank is 1-based index.
        arr[TO_ZERO_BASE(rank)] = temp_arr[i];
        counts[temp_arr[i] - min_ele] --;
    }
}

} // namespace sort_algo

