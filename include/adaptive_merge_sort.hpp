/**
 * @file adaptive_merge_sort.hpp
 * @brief  Class for adaptive merge sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <stack>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class AdaptiveMergeSort
{
    public:
        inline static const std::string name = "Adaptive Merge Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = false;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        AdaptiveMergeSort() = default;

        /* Same as merge method in MergeSort */
        // Note: [start, mid), [mid, end)
        template <typename T, typename Compare>
        static void merge(T* arr, std::size_t start, std::size_t mid, std::size_t end, Compare cmp);

        /* Same as get_run method in TimSort */
        template <typename T, typename Compare> 
        static std::size_t get_run(T* arr, const std::size_t n, std::size_t run_start, Compare cmp);
};

template <typename T, typename Compare>
void AdaptiveMergeSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
    
    std::size_t run_start = 0;
    std::vector<std::pair<std::size_t, std::size_t>> run_stack;

    while (run_start < n) {

        // The range of this run is from run_start to run_end (inclusive).
        std::size_t run_end = get_run(arr, n, run_start, cmp);
        run_stack.push_back({run_start, run_end});
        run_start = run_end + 1;

        /* Same as rule in TimSort */
        while (run_stack.size() >= 3) {

            std::size_t size = run_stack.size();

            auto [l_A, r_A] = run_stack[size - 3];
            auto [l_B, r_B] = run_stack[size - 2];
            auto [l_C, r_C] = run_stack[size - 1];

            std::size_t size_A = r_A - l_A + 1;
            std::size_t size_B = r_B - l_B + 1;
            std::size_t size_C = r_C - l_C + 1;

            if (size_A > size_B + size_C && size_B > size_C) break;

            /* Case 1: Merge A and B */
            if (size_A < size_C) {
                merge(arr, l_A, r_A + 1, r_B + 1, cmp);
                // Update run_stack
                for (int8_t _ = 0; _ < 3; ++_) run_stack.pop_back();
                run_stack.push_back({l_A, r_B});
                run_stack.push_back({l_C, r_C});
            }
            /* Case 2: Merge B and C */
            else {
                merge(arr, l_B, r_B + 1, r_C + 1, cmp);
                for (int8_t _ = 0; _ < 2; ++ _) run_stack.pop_back();
                run_stack.push_back({l_B, r_C});
            }
        }
    }

    // after finding all runs, merge them.
    while (run_stack.size() > 1) {

        std::size_t size = run_stack.size();

        auto [l_B, r_B] = run_stack[size - 2];
        auto [l_C, r_C] = run_stack[size - 1];

        merge(arr, l_B, r_B + 1, r_C + 1, cmp);
        for (int8_t _ = 0; _ < 2; ++ _) run_stack.pop_back();
        run_stack.push_back({l_B, r_C});
    }
}

template <typename T, typename Compare>
void AdaptiveMergeSort::merge(T* arr, std::size_t start, std::size_t mid, std::size_t end, Compare cmp)
{
    std::size_t n1 = mid - start;
    std::size_t n2 = end - mid;
    
    std::vector<T> l_vec(arr + start, arr + mid);
    std::vector<T> r_vec(arr + mid, arr + end);
    std::size_t i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if (cmp(r_vec[j], l_vec[i])) arr[k ++] = r_vec[j ++];
        else                         arr[k ++] = l_vec[i ++];
    }

    while (i < n1) arr[k ++] = l_vec[i ++];
    while (j < n2) arr[k ++] = r_vec[j ++];
}

template <typename T, typename Compare> 
std::size_t AdaptiveMergeSort::get_run(T* arr, const std::size_t n, std::size_t run_start, Compare cmp)
{
    std::size_t run_end = run_start;
    if (run_end + 1 >= n) return run_start;

    bool reverse = cmp(arr[run_end + 1], arr[run_end]);
    while (run_end + 1 < n && reverse == cmp(arr[run_end + 1], arr[run_end])) run_end ++;

    if (reverse) {
        for (ptrdiff_t l = run_start, r = run_end; l < r; ++l, --r) {
            std::swap(arr[l], arr[r]);
        }
    }

    return run_end;
}

} // namespace sort_imp

