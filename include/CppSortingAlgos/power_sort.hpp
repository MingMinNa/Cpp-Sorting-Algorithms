/**
 * @file power_sort.hpp
 * @brief  Class for power sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include <span>
#include <vector>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace sort_algo 
{

class PowerSort
{
    public:
        inline static const std::string name = "Power Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = false;
        
        inline static const uint8_t MIN_MERGE  = 64u;
        inline static const uint8_t MIN_GALLOP = 7u;

        template <typename T, typename Compare = std::less<T>>
        static void sort(std::span<T> arr_span, Compare cmp = Compare{});
        
    private:

        struct Run {
            std::size_t run_start;
            std::size_t run_end;
            std::size_t power;
        };

        PowerSort() = default;

        static std::size_t calc_min_run(std::size_t n);

        template <typename T, typename Compare>
        static std::size_t get_run(
            T* arr, std::size_t n, 
            std::size_t run_start, Compare cmp
        );

        static std::size_t power_loop(
            std::size_t run_start, 
            std::size_t n1,
            std::size_t n2,
            std::size_t n
        );

        template <typename T, typename Compare>
        static void found_new_run(
            T* arr, std::vector<Run> &run_stack, Compare cmp,
            std::size_t n2, std::size_t n, ptrdiff_t &min_gallop
        );

        template <typename T, typename Compare>
        static void power_merge(
            T* arr, std::size_t start, std::size_t mid, std::size_t end,
            ptrdiff_t& min_gallop, Compare cmp
        );

        template <typename T, typename Compare>
        static ptrdiff_t gallop_right(
            const T& key, T* arr, ptrdiff_t base,
            ptrdiff_t len, ptrdiff_t hint, Compare cmp
        );

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
        static void merge_force_collapse(
            T* arr, std::vector<Run> &run_stack, 
            ptrdiff_t &min_gallop, Compare cmp
        );
};

template <typename T, typename Compare>
void PowerSort::sort(std::span<T> arr_span, Compare cmp)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();
    
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::size_t min_run = calc_min_run(n);
    std::vector<Run> run_stack;
    std::size_t run_start = 0;
    ptrdiff_t min_gallop = MIN_GALLOP;

    while (run_start < n) {

        // The range of this run is from run_start to run_end (inclusive).
        std::size_t run_end  = get_run(arr, n, run_start, cmp);
        std::size_t run_size = run_end - run_start + 1;

        if (run_size < min_run) {
            run_size = std::min(min_run, n - run_start);
            run_end  = run_start + run_size - 1;
            BinaryInsertionSort::sort(std::span<T>{arr + run_start, run_size}, cmp);
        }

        found_new_run(
            arr, run_stack, cmp,
            run_size, n, min_gallop
        );

        run_stack.push_back({run_start, run_end, 0});
        run_start = run_end + 1;
    }

    // After finding all runs, merge them.
    merge_force_collapse(arr, run_stack, min_gallop, cmp);
}

std::size_t PowerSort::calc_min_run(std::size_t n)
{
    ptrdiff_t plus_one = 0;
    while (n >= MIN_MERGE) {
        plus_one |= (n & 1);
        n >>= 1;
    }
    return n + plus_one;
}

template <typename T, typename Compare>
std::size_t PowerSort::get_run(
    T* arr, const std::size_t n,
    std::size_t run_start, Compare cmp
) {
    // Assume cmp is less<T> in the following code.
    std::size_t run_end = run_start;

    for (; run_end + 1 < n; run_end ++) {
        if (cmp(arr[run_end + 1], arr[run_end])) 
            break;
    }
    
    // arr[run_start] ~ arr[run_end] are ascending.
    if (run_end + 1 >= n) return run_end;

    if (run_end - run_start >= 1) {
        // arr[run_start] < arr[run_end]
        if (cmp(arr[run_start], arr[run_end])) return run_end;
        
        // All elements from arr[run_start] to arr[run_end] are equal.
        std::reverse(arr + run_start, arr + run_end + 1);
    }

    ++ run_end; 
    std::size_t duplicate = 1;

    for (; run_end + 1 < n; run_end ++) {
        /* arr[run_end] < arr[run_end + 1] */
        // End of the descending interval.
        if (cmp(arr[run_end], arr[run_end + 1])) {
            break;
        }
        /* arr[run_end] == arr[run_end + 1] */
        else if (!cmp(arr[run_end + 1], arr[run_end])) {
            duplicate ++;
        }
        /* arr[run_end] > arr[run_end + 1] */
        // Reverse the duplicate elements.
        else if (duplicate >= 2) {
            std::reverse(arr + run_end + 1 - duplicate, arr + run_end + 1);
            duplicate = 1;
        }
    }

    if (duplicate >= 2) {
        std::reverse(arr + run_end + 1 - duplicate, arr + run_end + 1);
        duplicate = 1;
    }

    std::reverse(arr + run_start, arr + run_end + 1);

    // After reverse, arr[run_start] ~ arr[run_end] are ascending now.
    for (; run_end + 1 < n; run_end ++) {
        if (cmp(arr[run_end + 1], arr[run_end])) break;
    }

    return run_end;
}

std::size_t PowerSort::power_loop(
    std::size_t run_start, 
    std::size_t n1,
    std::size_t n2,
    std::size_t n
) {
    std::size_t ret = 0;
    
    // midpoint of run_1: a = run_start + n1 / 2;
    // midpoint of run_2: b = run_start + n1 + n2 / 2 = a + (n1 + n2) / 2;
    // But use 2a and 2b instead since 2a and 2b are integers.
    std::size_t double_a = 2 * run_start + n1;
    std::size_t double_b = double_a + n1 + n2;

    while (true) {
        ++ ret;
        if (double_a >= n) {
            double_a -= n;
            double_b -= n;
        }
        else if (double_b >= n) {
            break;
        }

        double_a <<= 1;
        double_b <<= 1;
    }

    return ret;
}

template <typename T, typename Compare>
void PowerSort::found_new_run(
    T* arr, std::vector<Run> &run_stack, Compare cmp,
    std::size_t n2, std::size_t n, ptrdiff_t &min_gallop
) {
    std::size_t size = run_stack.size();
    if (!size) return;

    std::size_t run_start = run_stack.back().run_start;
    std::size_t n1    = run_stack.back().run_end - run_start + 1;
    std::size_t power = power_loop(run_start, n1, n2, n);

    while (size >= 2 && run_stack[size - 2].power > power) {

        std::size_t l_B = run_stack[size - 2].run_start;
        std::size_t r_B = run_stack[size - 2].run_end;
        std::size_t r_C = run_stack[size - 1].run_end;
        
        size --;
        run_stack.pop_back();
        run_stack[size - 1].run_end = r_C; // From r_B to r_C
        power_merge(arr, l_B, r_B, r_C, min_gallop, cmp);
    }

    run_stack[size - 1].power = power;
}

template <typename T, typename Compare>
void PowerSort::power_merge(
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
ptrdiff_t PowerSort::gallop_right(
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
ptrdiff_t PowerSort::gallop_left(
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
void PowerSort::merge_lo(
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
void PowerSort::merge_hi(
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
void PowerSort::merge_force_collapse(
    T* arr, std::vector<Run> &run_stack, 
    ptrdiff_t &min_gallop, Compare cmp
) {
    while (run_stack.size() > 1) {

        std::size_t size = run_stack.size();
        std::size_t l_B = run_stack[size - 2].run_start;
        std::size_t r_B = run_stack[size - 2].run_end;
        std::size_t l_C = run_stack[size - 1].run_start;
        std::size_t r_C = run_stack[size - 1].run_end;

        if (size >= 3) {
            std::size_t l_A = run_stack[size - 3].run_start;
            std::size_t r_A = run_stack[size - 3].run_end;
            std::size_t size_A = r_A - l_A + 1;
            std::size_t size_C = r_C - l_C + 1;

            if (size_A < size_C) {
                power_merge(arr, l_A, r_A, r_B, min_gallop, cmp);
                run_stack.resize(run_stack.size() - 3);
                run_stack.push_back({l_A, r_B, 0});
                run_stack.push_back({l_C, r_C, 0});
                continue;
            }
        }

        power_merge(arr, l_B, r_B, r_C, min_gallop, cmp);
        run_stack.resize(run_stack.size() - 2);
        run_stack.push_back({l_B, r_C, 0});
    }
}

} // namespace sort_algo
