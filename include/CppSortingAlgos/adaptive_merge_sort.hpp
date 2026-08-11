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

namespace sort_algo 
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
        template <typename T, typename Compare>
        static void merge(T* arr, std::size_t start, std::size_t mid, std::size_t end, Compare cmp);

        /* Same as get_run method in TimSort */
        template <typename T, typename Compare> 
        static std::size_t get_run(
            T* arr, const std::size_t n, 
            std::size_t run_start, Compare cmp
        );

        template <typename T, typename Compare>
        static void merge_collapse(
            T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, Compare cmp
        );

        template <typename T, typename Compare>
        static void merge_force_collapse(
            T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, Compare cmp
        );
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
        merge_collapse(arr, run_stack, cmp);
    }

    // after finding all runs, merge them.
    merge_force_collapse(arr, run_stack, cmp);
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
std::size_t AdaptiveMergeSort::get_run(
    T* arr, const std::size_t n, 
    std::size_t run_start, Compare cmp
) {
    std::size_t run_end = run_start;
    if (run_end + 1 >= n) return run_start;

    if (cmp(arr[run_end + 1], arr[run_end])) {
        for (;run_end + 1 < n && cmp(arr[run_end + 1], arr[run_end]); ++ run_end);
        for (std::size_t l = run_start, r = run_end; l < r; ++l, --r) 
            std::swap(arr[l], arr[r]);
    } 
    else {
        for (;run_end + 1 < n && !cmp(arr[run_end + 1], arr[run_end]); ++ run_end);
    }
    return run_end;
}


template <typename T, typename Compare>
void AdaptiveMergeSort::merge_collapse(
    T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, Compare cmp
) {
    bool changed = true;
    while (changed && run_stack.size() >= 2) {

        changed = false;
        std::size_t size = run_stack.size();

        auto [l_B, r_B] = run_stack[size - 2];
        auto [l_C, r_C] = run_stack[size - 1];
        std::size_t size_B = r_B - l_B + 1;
        std::size_t size_C = r_C - l_C + 1;

        if (size >= 3) {
            auto [l_A, r_A] = run_stack[size - 3];
            std::size_t size_A = r_A - l_A + 1;
            bool upper_violated = false;

            if (size >= 4) {
                auto [l_X, r_X] = run_stack[size - 4];
                std::size_t size_X = r_X - l_X + 1;
                upper_violated = (size_X <= size_A + size_B);
            }

            if ((size_A <= size_B + size_C || upper_violated)) {
                /* Case 1: Merge A and B */
                if (size_A < size_C) {
                    merge(arr, l_A, r_A + 1, r_B + 1, cmp);
                    run_stack.pop_back();
                    run_stack.pop_back();
                    run_stack.pop_back();
                    run_stack.push_back({l_A, r_B});
                    run_stack.push_back({l_C, r_C});
                }
                /* Case 2: Merge B and C */
                else {
                    merge(arr, l_B, r_B + 1, r_C + 1, cmp);
                    run_stack.pop_back();
                    run_stack.pop_back();
                    run_stack.push_back({l_B, r_C});
                }
                changed = true;
            }
            /* Case 2: Merge B and C */
            else if (size_B <= size_C) {
                merge(arr, l_B, r_B + 1, r_C + 1, cmp);
                run_stack.pop_back();
                run_stack.pop_back();
                run_stack.push_back({l_B, r_C});
                changed = true;
            }
        }
        // Invariant #2
        else if (size_B <= size_C){
            merge(arr, l_B, r_B + 1, r_C + 1, cmp);
            run_stack.pop_back();
            run_stack.pop_back();
            run_stack.push_back({l_B, r_C});
            changed = true;
        }
    }
}

template <typename T, typename Compare>
void AdaptiveMergeSort::merge_force_collapse(
    T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, Compare cmp
) {
    while (run_stack.size() > 1) {

        std::size_t size = run_stack.size();
        auto [l_B, r_B] = run_stack[size - 2];
        auto [l_C, r_C] = run_stack[size - 1];

        if (size >= 3) {
            auto [l_A, r_A] = run_stack[size - 3];
            std::size_t size_A = r_A - l_A + 1;
            std::size_t size_C = r_C - l_C + 1;

            if (size_A < size_C) {
                merge(arr, l_A, r_A + 1, r_B + 1, cmp);
                run_stack.pop_back();
                run_stack.pop_back();
                run_stack.pop_back();
                run_stack.push_back({l_A, r_B});
                run_stack.push_back({l_C, r_C});
                continue;
            }
        }

        merge(arr, l_B, r_B + 1, r_C + 1, cmp);
        run_stack.pop_back();
        run_stack.pop_back();
        run_stack.push_back({l_B, r_C});
    }
}

} // namespace sort_algo

