/**
 * @file tournament_sort.hpp
 * @brief  Class for tournament sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <vector>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class TournamentSort
{
    public:
        TournamentSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return false; }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return false; }

    private:

        // Check whether arr[a] wins arr[b].
        template <typename T, typename Compare>
        static bool beats(const T* arr, size_t n, size_t a, size_t b, Compare cmp);

        template <typename T, typename Compare>
        static void update(
            std::vector<size_t>& tree,
            const T* arr, size_t n,
            size_t leaf_index, Compare cmp
        );
};

template <typename T, typename Compare>
void TournamentSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    size_t leaves = 1u;
    for (; leaves < n; leaves <<= 1);
    size_t tree_size = leaves << 1;

    std::vector<T>      temp_arr(arr, arr + n);
    std::vector<size_t> tree(tree_size, std::numeric_limits<size_t>::max());

    for (size_t i = 0; i < n; ++i) {
        tree[leaves + i] = i;
    }

    for (size_t i = leaves - 1; i >= 1; --i) {
        size_t left  = tree[2 * i];
        size_t right = tree[2 * i + 1];
        tree[i] = beats(temp_arr.data(), n, left, right, cmp) ? left : right;
    }

    for (size_t i = 0; i < n; ++i) {
        size_t winner = tree[1];
        arr[i] = temp_arr[winner];

        size_t leaf_index = leaves + winner;
        tree[leaf_index] = std::numeric_limits<size_t>::max(); 
        update(tree, temp_arr.data(), n, leaf_index, cmp);
    }
}

template <typename T, typename Compare>
bool TournamentSort::beats(const T* arr, size_t n, size_t a, size_t b, Compare cmp)
{
    if (a >= n) return false;    // a is empty → a loses
    if (b >= n) return true;     // b is empty → a wins
    return !cmp(arr[b], arr[a]); // a wins when arr[b] does not beat arr[a]
}

template <typename T, typename Compare>
void TournamentSort::update(
    std::vector<size_t>& tree,
    const T* arr, size_t n,
    size_t index, Compare cmp
) {
    index >>= 1;

    while (index >= 1) {
        size_t left  = tree[2 * index];
        size_t right = tree[2 * index + 1];
        tree[index] = beats(arr, n, left, right, cmp) ? left : right;
        index >>= 1;
    }
}

} // namespace sort_imp

