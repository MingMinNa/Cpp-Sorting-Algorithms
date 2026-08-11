/**
 * @file tim_sort.hpp
 * @brief  Class for Tim sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include <vector>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace sort_algo
{

class TimSort
{
    public:
        inline static const std::string name = "Tim Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = false;

        inline static const uint8_t MIN_MERGE  = 64u;
        inline static const uint8_t MIN_GALLOP = 7u;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        TimSort() = default;

        static std::size_t calc_min_run(std::size_t n);

        template <typename T, typename Compare>
        static std::size_t get_run(
            T* arr, std::size_t n, 
            std::size_t run_start, Compare cmp
        );

        template <typename T, typename Compare>
        static void tim_merge(
            T* arr, std::size_t start, std::size_t mid, std::size_t end,
            ptrdiff_t& min_gallop, Compare cmp
        );

        // find rightmost position in [base, base + len) where key can be inserted to keep the array sorted; 
        // i.e. the first index i such that arr[base + i] > key.
        template <typename T, typename Compare>
        static ptrdiff_t gallop_right(
            const T& key, T* arr, ptrdiff_t base,
            ptrdiff_t len, ptrdiff_t hint, Compare cmp
        );

        // find leftmost position in [base, base + len) where key can be inserted; 
        // i.e. the first index i such that arr[base + i] >= key.
        template <typename T, typename Compare>
        static ptrdiff_t gallop_left(
            const T& key, T* arr, ptrdiff_t base,
            ptrdiff_t len, ptrdiff_t hint, Compare cmp
        );

        template <typename T, typename Compare>
        static void merge_lo(
            T* arr, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end,
            ptrdiff_t& min_gallop, Compare cmp
        );

        template <typename T, typename Compare>
        static void merge_hi(
            T* arr, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end,
            ptrdiff_t& min_gallop, Compare cmp
        );

        template <typename T, typename Compare>
        static void merge_collapse(
            T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, 
            ptrdiff_t &min_gallop, Compare cmp
        );

        template <typename T, typename Compare>
        static void merge_force_collapse(
            T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, 
            ptrdiff_t &min_gallop, Compare cmp
        );
};

template <typename T, typename Compare>
void TimSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t min_run = calc_min_run(n);
    std::vector<std::pair<std::size_t, std::size_t>> run_stack;

    ptrdiff_t min_gallop = MIN_GALLOP;
    std::size_t run_start = 0;

    while (run_start < n) {

        // The range of this run is from run_start to run_end (inclusive).
        std::size_t run_end  = get_run(arr, n, run_start, cmp);
        std::size_t run_size = run_end - run_start + 1;

        if (run_size < min_run) {
            run_size = std::min(min_run, n - run_start);
            run_end  = run_start + run_size - 1;
            BinaryInsertionSort::sort(arr + run_start, run_size, cmp);
        }

        run_stack.push_back({run_start, run_end});
        run_start = run_end + 1;
        merge_collapse(arr, run_stack, min_gallop, cmp);
    }

    // After finding all runs, merge them.
    merge_force_collapse(arr, run_stack, min_gallop, cmp);
}

std::size_t TimSort::calc_min_run(std::size_t n)
{
    ptrdiff_t plus_one = 0;
    while (n >= MIN_MERGE) {
        plus_one |= (n & 1);
        n >>= 1;
    }
    return n + plus_one;
}

template <typename T, typename Compare>
std::size_t TimSort::get_run(
    T* arr, const std::size_t n,
    std::size_t run_start, Compare cmp
) {
    std::size_t run_end = run_start;
    if (run_end + 1 >= n) return run_start;

    if (cmp(arr[run_end + 1], arr[run_end])) {
        for (;run_end + 1 < n && cmp(arr[run_end + 1], arr[run_end]); ++ run_end);
        std::reverse(arr + run_start, arr + run_end + 1);
    } 
    else {
        for (;run_end + 1 < n && !cmp(arr[run_end + 1], arr[run_end]); ++ run_end);
    }
    return run_end;
}

template <typename T, typename Compare>
void TimSort::tim_merge(
    T* arr, std::size_t start, std::size_t mid, std::size_t end, 
    ptrdiff_t& min_gallop, Compare cmp
) {
    std::size_t n1 = mid - start + 1;
    std::size_t n2 = end - mid;
    
    if (n1 == 0 || n2 == 0) return;

    ptrdiff_t k = gallop_right(arr[mid + 1], arr, start, n1, 0, cmp);
    
    std::size_t new_start = start + static_cast<std::size_t>(k);
    std::size_t new_n1 = n1 - static_cast<std::size_t>(k);

    if (new_n1 == 0) return;

    ptrdiff_t j = gallop_left(arr[mid], arr, mid + 1, n2, static_cast<ptrdiff_t>(n2) - 1, cmp);

    std::size_t new_n2 = static_cast<std::size_t>(j);

    if (new_n2 == 0) return;

    std::size_t new_mid = new_start + new_n1 - 1;
    std::size_t new_end = new_mid + new_n2;

    if (new_n1 <= new_n2) merge_lo(arr, new_start, new_mid, new_end, min_gallop, cmp);
    else                  merge_hi(arr, new_start, new_mid, new_end, min_gallop, cmp);
}

template <typename T, typename Compare>
ptrdiff_t TimSort::gallop_right(
    const T& key, T* arr, ptrdiff_t base,
    ptrdiff_t len, ptrdiff_t hint, Compare cmp
) {
    // arr[base + hint] is our starting probe.
    ptrdiff_t ofs     = 1;
    ptrdiff_t lastOfs = 0;
    ptrdiff_t lo, hi;

    // key < arr[base + hint]: gallop left.
    if (cmp(key, arr[base + hint])) {
        ptrdiff_t maxOfs = hint + 1;
        while (ofs < maxOfs && cmp(key, arr[base + hint - ofs])) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
        }
        ofs = std::min(ofs, maxOfs);
        lo = hint - ofs;
        hi = hint - lastOfs;
    } 
    // key >= arr[base + hint]: gallop right.
    else {
        ptrdiff_t maxOfs = len - hint;
        while (ofs < maxOfs && !cmp(key, arr[base + hint + ofs])) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
        }
        ofs = std::min(ofs, maxOfs);
        lo = hint + lastOfs;
        hi = hint + ofs;
    }
    
    // Binary search: find first k in (lo, hi] with arr[base + k] > key.
    ++ lo;
    while (lo < hi) {
        ptrdiff_t mid = lo + ((hi - lo) >> 1);
        if (cmp(key, arr[base + mid])) hi = mid;
        else                           lo = mid + 1;
    }
    return hi;
}

template <typename T, typename Compare>
ptrdiff_t TimSort::gallop_left(
    const T& key, T* arr, ptrdiff_t base,
    ptrdiff_t len, ptrdiff_t hint, Compare cmp
) {
    ptrdiff_t ofs     = 1;
    ptrdiff_t lastOfs = 0;
    ptrdiff_t lo, hi;

    // arr[base + hint] < key: gallop right.
    if (cmp(arr[base + hint], key)) {
        ptrdiff_t maxOfs = len - hint;
        while (ofs < maxOfs && cmp(arr[base + hint + ofs], key)) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
        }
        ofs = std::min(ofs, maxOfs);
        lo = hint + lastOfs;
        hi = hint + ofs;
    }
    // arr[base + hint] >= key: gallop left. 
    else {
        ptrdiff_t maxOfs = hint + 1;
        while (ofs < maxOfs && !cmp(arr[base + hint - ofs], key)) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
        }
        ofs = std::min(ofs, maxOfs);
        lo = hint - ofs;
        hi = hint - lastOfs;
    }
    
    ++ lo;
    while (lo < hi) {
        ptrdiff_t mid = lo + ((hi - lo) >> 1);
        if (cmp(arr[base + mid], key)) lo = mid + 1;
        else                           hi = mid;
    }
    return hi;
}

template <typename T, typename Compare>
void TimSort::merge_lo(
    T* arr, ptrdiff_t start, ptrdiff_t mid,
    ptrdiff_t end, ptrdiff_t& min_gallop, Compare cmp
) {
    ptrdiff_t n1 = mid - start + 1;
    std::vector<T> left(arr + start, arr + mid + 1);

    ptrdiff_t i = 0;       // cursor in left[]
    ptrdiff_t j = mid + 1; // cursor in arr (right half)
    ptrdiff_t k = start;   // write cursor

    ptrdiff_t count_left  = 0;
    ptrdiff_t count_right = 0;
    ptrdiff_t mg = min_gallop;

    while (i < n1 && j <= end) {

        if (cmp(arr[j], left[i])) {
            arr[k] = arr[j];
            k ++, j ++;
            count_left = 0;
            count_right ++;
        } 
        else {
            arr[k] = left[i];
            k ++, i ++;
            count_left ++;
            count_right = 0;
        }
        
        if (i >= n1 || j > end) goto done;

        // Enter gallop mode when one side wins mg consecutive times.
        if (count_left >= mg || count_right >= mg) {

            ++ mg;
            do {
                mg -= (mg > 1);
                min_gallop = mg;

                ptrdiff_t adv_l = gallop_right(arr[j], left.data(), i, n1 - i, 0, cmp);
                count_left = adv_l;

                if (adv_l > 0) {
                    std::copy(left.data() + i, left.data() + i + adv_l, arr + k);
                    k += adv_l;
                    i += adv_l;
                }

                arr[k ++]  = arr[j ++];
                if (i >= n1 || j > end) goto done;

                ptrdiff_t adv_r = gallop_left(left[i], arr, j, end - j + 1, 0, cmp);
                count_right = adv_r;

                if (adv_r > 0) {
                    std::copy(arr + j, arr + j + adv_r, arr + k);
                    k += adv_r;
                    j += adv_r;
                }

                arr[k ++] = left[i ++];
                if (i >= n1 || j > end) goto done;

            } while (count_left >= MIN_GALLOP || count_right >= MIN_GALLOP);

            // Penalise a gallop that became unproductive.
            ++ mg;
            min_gallop = mg;
            count_left = count_right = 0;
        }
    }

done:
    // Copy remaining left[] elements.
    if (i < n1) std::copy(left.data() + i, left.data() + n1, arr + k);
}

template <typename T, typename Compare>
void TimSort::merge_hi(
    T* arr, ptrdiff_t start, ptrdiff_t mid,
    ptrdiff_t end, ptrdiff_t& min_gallop, Compare cmp
) {
    ptrdiff_t n2 = end - mid;
    std::vector<T> right(arr + mid + 1, arr + end + 1);

    ptrdiff_t i = mid;     // cursor in arr (left half), scanning right-to-left
    ptrdiff_t j = n2 - 1;  // cursor in right[], scanning right-to-left
    ptrdiff_t k = end;     // write cursor

    ptrdiff_t count_left  = 0;
    ptrdiff_t count_right = 0;
    ptrdiff_t mg = min_gallop;

    while (i >= start && j >= 0) {

        if (!cmp(right[j], arr[i])) {
            arr[k] = right[j];
            k --, j --;
            count_left = 0;
            ++ count_right;
        } 
        else {
            arr[k] = arr[i];
            k --, i --;
            ++ count_left;
            count_right = 0;
        }
        
        if (i < start || j < 0) goto done;

        if (count_left >= mg || count_right >= mg) {
            
            ++ mg;
            do {
                mg -= (mg > 1);
                min_gallop = mg;

                ptrdiff_t adv_l 
                    = (i - start + 1) - gallop_right(
                        right[j], arr, start, i - start + 1, i - start, cmp
                    );
                count_left = adv_l;

                if (adv_l > 0) {
                    k -= adv_l;
                    i -= adv_l;
                    std::copy(
                        arr + i + 1, 
                        arr + i + 1 + adv_l, 
                        arr + k + 1
                    );
                }
                
                arr[k --] = right[j --];
                if (i < start || j < 0) goto done;

                ptrdiff_t adv_r 
                    = (j + 1) - gallop_left(
                        arr[i], right.data(), 0, j + 1, j, cmp
                    );
                count_right = adv_r;

                if (adv_r > 0) {
                    k -= adv_r;
                    j -= adv_r;
                    std::copy(
                        right.data() + j + 1,
                        right.data() + j + 1 + adv_r, 
                        arr + k + 1
                    );
                }

                arr[k --] = arr[i --];
                if (i < start || j < 0) goto done;

            } while (count_left >= MIN_GALLOP || count_right >= MIN_GALLOP);
            
            ++ mg;
            min_gallop = mg;
            count_left = count_right = 0;
        }
    }

done:
    // Copy remaining right[] elements.
    if (j >= 0) std::copy(right.data(), right.data() + j + 1, arr + k - j);
}

template <typename T, typename Compare>
void TimSort::merge_collapse(
    T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, 
    ptrdiff_t &min_gallop, Compare cmp
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
                    tim_merge(arr, l_A, r_A, r_B, min_gallop, cmp);
                    run_stack.resize(run_stack.size() - 3);
                    run_stack.push_back({l_A, r_B});
                    run_stack.push_back({l_C, r_C});
                }
                /* Case 2: Merge B and C */
                else {
                    tim_merge(arr, l_B, r_B, r_C, min_gallop, cmp);
                    run_stack.resize(run_stack.size() - 2);
                    run_stack.push_back({l_B, r_C});
                }
                changed = true;
            }
            /* Case 2: Merge B and C */
            else if (size_B <= size_C) {
                tim_merge(arr, l_B, r_B, r_C, min_gallop, cmp);
                run_stack.resize(run_stack.size() - 2);
                run_stack.push_back({l_B, r_C});
                changed = true;
            }
        }
        // Invariant #2
        else if (size_B <= size_C){
            tim_merge(arr, l_B, r_B, r_C, min_gallop, cmp);
            run_stack.resize(run_stack.size() - 2);
            run_stack.push_back({l_B, r_C});
            changed = true;
        }
    }
}

template <typename T, typename Compare>
void TimSort::merge_force_collapse(
    T* arr, std::vector<std::pair<std::size_t, std::size_t>> &run_stack, 
    ptrdiff_t &min_gallop, Compare cmp
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
                tim_merge(arr, l_A, r_A, r_B, min_gallop, cmp);
                run_stack.resize(run_stack.size() - 3);
                run_stack.push_back({l_A, r_B});
                run_stack.push_back({l_C, r_C});
                continue;
            }
        }

        tim_merge(arr, l_B, r_B, r_C, min_gallop, cmp);
        run_stack.resize(run_stack.size() - 2);
        run_stack.push_back({l_B, r_C});
    }
}

} // namespace sort_algo