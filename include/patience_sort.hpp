/**
 * @file patience_sort.hpp
 * @brief  Class for patience sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <queue>
#include <vector>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class PatienceSort
{
    public:
        PatienceSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return false; }

    private:

        template <typename T, typename Compare>
        static size_t binary_search(
            const std::vector<std::vector<T>> &piles, const T &ele, Compare cmp
        );
};

template <typename T, typename Compare>
void PatienceSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::vector<std::vector<T>> piles;
 
    for (size_t i = 0; i < n; ++i) {
        size_t index = binary_search(piles, arr[i], cmp);
        if (index >= piles.size())  piles.push_back({});
        piles[index].push_back(arr[i]);   // push onto pile top
    }

    // first : element, 
    // second: pile index, 
    // third : element index
    using Entry = std::tuple<T, size_t, size_t>;
 
    auto heap_cmp = [&](const Entry &a, const Entry &b) {
        return cmp(std::get<0>(b), std::get<0>(a));
    };
 
    std::priority_queue<Entry, std::vector<Entry>, decltype(heap_cmp)> pq(heap_cmp);
 
    for (size_t i = 0; i < piles.size(); ++i) {
        if (!piles[i].empty()) {
            size_t top_index = piles[i].size() - 1;
            pq.emplace(piles[i][top_index], i, top_index);
        }
    }
 
    size_t index = 0;
    while (!pq.empty()) {
        auto [ele, pile_index, ele_index] = pq.top(); pq.pop();
        arr[index ++] = ele;

        if (ele_index > 0) {
            size_t next = ele_index - 1;
            pq.emplace(piles[pile_index][next], pile_index, next);
        }
    }
}

template <typename T, typename Compare>
size_t PatienceSort::binary_search(
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

} // namespace sort_imp

