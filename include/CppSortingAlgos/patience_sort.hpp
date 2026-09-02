/**
 * @file patience_sort.hpp
 * @brief  Class for patience sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <span>
#include <string>
#include <queue>
#include <vector>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class PatienceSort
{
    public:
        inline static const std::string name = "Patience Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = false;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});

    private:
        PatienceSort() = default;

        template <typename T, typename Compare>
        static std::size_t binary_search(
            const std::vector<std::vector<T>> &piles, const T &ele, Compare cmp
        );
};

template <typename T, typename Compare>
void PatienceSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();
    
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::vector<std::vector<T>> piles;
 
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t index = binary_search(piles, arr[i], cmp);
        if (index >= piles.size())  piles.push_back({});
        piles[index].push_back(arr[i]);   // push onto pile top
    }

    // first : element, 
    // second: pile index, 
    // third : element index
    using Entry = std::tuple<T, std::size_t, std::size_t>;
 
    auto heap_cmp = [&](const Entry &a, const Entry &b) {
        return cmp(std::get<0>(b), std::get<0>(a));
    };
 
    std::priority_queue<
        Entry, std::vector<Entry>, decltype(heap_cmp)
    > pq(heap_cmp);
 
    for (std::size_t i = 0; i < piles.size(); ++i) {
        if (!piles[i].empty()) {
            std::size_t top_index = piles[i].size() - 1;
            pq.emplace(piles[i][top_index], i, top_index);
        }
    }
 
    std::size_t index = 0;
    while (!pq.empty()) {
        auto [ele, pile_index, ele_index] = pq.top(); pq.pop();
        arr[index ++] = ele;

        if (ele_index > 0) {
            std::size_t next = ele_index - 1;
            pq.emplace(piles[pile_index][next], pile_index, next);
        }
    }
}

template <typename T, typename Compare>
std::size_t PatienceSort::binary_search(
    const std::vector<std::vector<T>> &piles, const T &ele, Compare cmp
) {
     if (piles.empty() || cmp(piles.back().back(), ele))
        return piles.size();

    ptrdiff_t index = -1, size = piles.size();

    for (ptrdiff_t jump = size >> 1; jump >= 1; jump >>= 1) {
        while (index + jump < size && cmp(piles[index + jump].back(), ele)) {
            index += jump;
        }
    }

    return index + 1;
}

} // namespace sort_algo

