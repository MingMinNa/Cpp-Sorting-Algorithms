/**
 * @file tournament_sort.hpp
 * @brief  Class for tournament sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <bit>
#include <string>
#include <vector>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class TournamentSort
{
    public:
        inline static const std::string name = "Tournament Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = false;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        TournamentSort() = default;

        // Check whether arr[a] wins arr[b].
        template <typename T, typename Compare>
        static bool beats(const T* arr, std::size_t n, std::size_t a, std::size_t b, Compare cmp);

        template <typename T, typename Compare>
        static void update(
            std::vector<std::size_t>& tree,
            const T* arr, std::size_t n,
            std::size_t leaf_index, Compare cmp
        );
};

template <typename T, typename Compare>
void TournamentSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t leaves = std::bit_ceil(n);
    std::size_t tree_size = leaves + n;
    leaves = n;

    std::vector<T> temp_arr(arr, arr + n);
    std::vector<std::size_t> tree(tree_size, std::numeric_limits<std::size_t>::max());

    for (std::size_t i = 0; i < n; ++i) {
        tree[tree_size - leaves + i] = i;
    }

    std::size_t l_range = tree_size - leaves; 
    std::size_t r_range = tree_size - 1;
    std::size_t left, right;

    l_range >>= 1;
    r_range >>= 1;

    for (std::size_t i = l_range; i <= r_range; ++i) {
        left  = tree[i << 1];
        right = tree[std::min((i << 1) + 1, tree_size - 1)];
        tree[i] = beats(temp_arr.data(), n, left, right, cmp) ? left : right;
    }

    l_range >>= 1;
    r_range >>= 1;

    while (l_range >= 1) {
        for (std::size_t i = l_range; i <= r_range; ++i) {
            left  = tree[i << 1];
            right = tree[(i << 1) + 1];
            tree[i] = beats(temp_arr.data(), n, left, right, cmp) ? left : right;
        }
        l_range >>= 1;
        r_range >>= 1;
    }

    for (std::size_t i = 0; i < n; ++i) {
        std::size_t winner = tree[1];
        arr[i] = temp_arr[winner];

        std::size_t leaf_index = tree_size - leaves + winner;
        tree[leaf_index] = std::numeric_limits<std::size_t>::max(); 
        update(tree, temp_arr.data(), n, leaf_index, cmp);
    }
}

template <typename T, typename Compare>
bool TournamentSort::beats(const T* arr, std::size_t n, std::size_t a, std::size_t b, Compare cmp)
{
    if (a >= n) return false;    // a is empty → a loses
    if (b >= n) return true;     // b is empty → a wins
    return !cmp(arr[b], arr[a]); // a wins when arr[b] does not beat arr[a]
}

template <typename T, typename Compare>
void TournamentSort::update(
    std::vector<std::size_t>& tree,
    const T* arr, std::size_t n,
    std::size_t index, Compare cmp
) {
    std::size_t tree_size = tree.size();
    index >>= 1;
    
    std::size_t left  = tree[index << 1];
    std::size_t right = tree[std::min((index << 1) + 1, tree_size - 1)];
    tree[index] = beats(arr, n, left, right, cmp) ? left : right;
    index >>= 1;

    while (index >= 1) {
        left  = tree[index << 1];
        right = tree[(index << 1) + 1];
        tree[index] = beats(arr, n, left, right, cmp) ? left : right;
        index >>= 1;
    }
}

} // namespace sort_algo

