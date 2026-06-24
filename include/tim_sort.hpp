/**
 * @file tim_sort.hpp
 * @brief  Class for Tim sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class TimSort
{
    public:
        static const uint8_t MIN_MERGE = 64u;
        static const uint8_t MIN_GALLOP = 7u;
        TimSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{});
        static inline bool is_stable()      { return true;  }
        static inline bool is_comparison()  { return true;  }
        static inline bool in_place()       { return false; }

    private:
        static size_t calc_min_run(size_t n);

        template <typename T, typename Compare> 
        static size_t get_run(T* arr, const size_t n, size_t run_start, Compare cmp);

        template <typename T, typename Compare>
        static void tim_merge(T* arr, size_t start, size_t mid, size_t end, Compare cmp);

        template <typename T, typename Compare>
        static ptrdiff_t merge_lo_left(T* arr, std::vector<T> &left, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp);

        template <typename T, typename Compare>
        static ptrdiff_t merge_lo_right(T* arr, std::vector<T> &left, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp);

        template <typename T, typename Compare>
        static void merge_lo(T* arr, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, Compare cmp);

        template <typename T, typename Compare>
        static ptrdiff_t merge_hi_left(T* arr, std::vector<T> &right, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp);

        template <typename T, typename Compare>
        static ptrdiff_t merge_hi_right(T* arr, std::vector<T> &right, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp);

        template <typename T, typename Compare>
        static void merge_hi(T* arr, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, Compare cmp);
};

template <typename T, typename Compare>
void TimSort::sort(T* arr, size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    size_t run_start = 0;
    size_t min_run = calc_min_run(n);
    std::vector<std::pair<size_t, size_t>> run_stack;

    while (run_start < n) {
        
        // The range of this run is from run_start to run_end (inclusive).
        size_t run_end = get_run(arr, n, run_start, cmp);
        size_t run_size = run_end - run_start + 1;

        if (run_size < min_run) {
            run_size = std::min(min_run, n - run_start);
            run_end = run_start + run_size - 1;
            InsertionSort::sort(arr + run_start, run_size, cmp);
        }
        
        run_stack.push_back({run_start, run_end});
        run_start = run_end + 1;

        // try to merge 
        while (run_stack.size() >= 3) {

            size_t size = run_stack.size();
            
            auto [l_A, r_A] = run_stack[size - 3];
            auto [l_B, r_B] = run_stack[size - 2];
            auto [l_C, r_C] = run_stack[size - 1];

            size_t size_A = r_A - l_A + 1;
            size_t size_B = r_B - l_B + 1;
            size_t size_C = r_C - l_C + 1;

            if (size_A > size_B + size_C && size_B > size_C) break;

            /* Case 1: Merge A and B */
            if (size_A < size_C) {
                tim_merge(arr, l_A, r_A, r_B, cmp);
                // Update run_stack
                for (int8_t _ = 0; _ < 3; ++_) run_stack.pop_back();
                run_stack.push_back({l_A, r_B});
                run_stack.push_back({l_C, r_C});
            }
            /* Case 2: Merge B and C */
            else {
                tim_merge(arr, l_B, r_B, r_C, cmp);
                // Update run_stack
                for (int8_t _ = 0; _ < 2; ++ _) run_stack.pop_back();
                run_stack.push_back({l_B, r_C});
            }
        }
    }

    // after finding all runs, merge them.
    while (run_stack.size() > 1) {
        size_t size = run_stack.size();

        auto [l_B, r_B] = run_stack[size - 2];
        auto [l_C, r_C] = run_stack[size - 1];

        tim_merge(arr, l_B, r_B, r_C, cmp);
        for (int8_t _ = 0; _ < 2; ++ _) run_stack.pop_back();
        run_stack.push_back({l_B, r_C});
    }
}

size_t TimSort::calc_min_run(size_t n)
{
    ptrdiff_t plus_one = 0u;
    while (n >= MIN_MERGE) {
        plus_one |= (n & 1);
        n >>= 1;
    }
    return n + plus_one;
}

template <typename T, typename Compare> 
size_t TimSort::get_run(T* arr, const size_t n, size_t run_start, Compare cmp)
{
    size_t run_end = run_start;
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

template <typename T, typename Compare>
void TimSort::tim_merge(T* arr, size_t start, size_t mid, size_t end, Compare cmp)
{
    // merge [start, mid] and [mid + 1, end]
    size_t n1 = mid - start + 1;
    size_t n2 = end - mid;

    if (n1 == 0 || n2 == 0) return;

    if (n1 <= n2) merge_lo<T>(arr, start, mid, end, cmp); 
    else          merge_hi<T>(arr, start, mid, end, cmp);
}

template <typename T, typename Compare>
ptrdiff_t TimSort::merge_lo_left(
    T* arr, std::vector<T> &left, 
    ptrdiff_t start, ptrdiff_t mid, [[maybe_unused]] ptrdiff_t end, 
    ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp
) {
    ptrdiff_t n1 = mid - start + 1;
    ptrdiff_t index = i - 1;

    for (ptrdiff_t jump = std::max((ptrdiff_t) 1, (n1 - i) >> 1); jump >= 1; jump >>= 1) {
        while (index + jump < n1 && !cmp(arr[j], left[index + jump])) {
            index += jump;
        }
    }

    ptrdiff_t ret = index + 1 - i;
    for (; i <= index; ++i, ++k) {
        arr[k] = left[i];
    }

    arr[k] = arr[j];
    k ++, j ++;
    return ret;
}

template <typename T, typename Compare>
ptrdiff_t TimSort::merge_lo_right(
    T* arr, std::vector<T> &left, 
    [[maybe_unused]] ptrdiff_t start, [[maybe_unused]] ptrdiff_t mid, ptrdiff_t end, 
    ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp
) {
    ptrdiff_t index = j - 1;
    
    for (ptrdiff_t jump = std::max((ptrdiff_t) 1, (end - j + 1) >> 1); jump >= 1; jump >>= 1) {
        while (index + jump <= end && cmp(arr[index + jump], left[i])) {
            index += jump;
        }
    }

    ptrdiff_t ret = index + 1 - j;
    for(; j <= index; ++j, ++k) {
        arr[k] = arr[j];
    }

    arr[k] = left[i];
    k ++, i ++;
    return ret;
}

template <typename T, typename Compare>
void TimSort::merge_lo(T* arr, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, Compare cmp)
{
    ptrdiff_t n1 = mid - start + 1;

    // copy left run
    std::vector<T> left(arr + start, arr + mid + 1);

    ptrdiff_t i = 0;           // index in left
    ptrdiff_t j = mid + 1;     // index in right
    ptrdiff_t k = start;       // merged index

    ptrdiff_t count_left = 0;
    ptrdiff_t count_right = 0;

    bool round = true, mode_on = false;

    while (i < n1 && j <= end) {

        if (!cmp(arr[j], left[i])) {
            arr[k] = left[i];
            k ++, i ++;
            count_left ++;
            count_right = 0;
            round = true;
        }
        else {
            arr[k] = arr[j];
            k ++, j ++;
            count_left = 0;
            count_right ++;
            round = false;
        }

        while (i < n1 && j <= end && (count_left | count_right) >= MIN_GALLOP) {
            count_left = count_right = 0;
            if (round)   count_left  = merge_lo_left(arr, left, start, mid, end, i, j, k, cmp);
            else         count_right = merge_lo_right(arr, left, start, mid, end, i, j, k, cmp);
            round = !round;
            mode_on = true;
        }

        if (mode_on) {
            mode_on = false;
            count_left = count_right = 0;
        }
    }

    while (i < n1) {
        arr[k] = left[i];
        k ++, i ++;
    }
}

template <typename T, typename Compare>
ptrdiff_t TimSort::merge_hi_left(
    T* arr, std::vector<T> &right, 
    ptrdiff_t start, [[maybe_unused]] ptrdiff_t mid, [[maybe_unused]] ptrdiff_t end, 
    ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp
) {
    ptrdiff_t index = i + 1;

    for (ptrdiff_t jump = std::max((ptrdiff_t) 1, (i - start + 1) >> 1); jump >= 1; jump >>= 1) {
        while (index >= start + jump && cmp(right[j], arr[index - jump])) {
            index -= jump;
        }
    }

    ptrdiff_t ret = i + 1 - index;
    for (; i >= index; --i, --k) {
        arr[k] = arr[i];
    }

    arr[k] = right[j];
    k --, j --;
    return ret;
}

template <typename T, typename Compare>
ptrdiff_t TimSort::merge_hi_right(
    T* arr, std::vector<T> &right, 
    [[maybe_unused]] ptrdiff_t start, [[maybe_unused]] ptrdiff_t mid, [[maybe_unused]] ptrdiff_t end, 
    ptrdiff_t &i, ptrdiff_t &j, ptrdiff_t &k, Compare cmp
) {
    ptrdiff_t index = j + 1; 

    for (ptrdiff_t jump = std::max((ptrdiff_t) 1, (j + 1) >> 1); jump >= 1; jump >>= 1) {
        while (index - jump >= 0 && !cmp(right[index - jump], arr[i])) {
            index -= jump;
        }
    }

    ptrdiff_t ret = j + 1 - index;
    for (; j >= index; --j, --k) {
        arr[k] = right[j];
    }

    arr[k] = arr[i];
    k --, i --;
    return ret;
}

template <typename T, typename Compare>
void TimSort::merge_hi(T* arr, ptrdiff_t start, ptrdiff_t mid, ptrdiff_t end, Compare cmp)
{
    ptrdiff_t n2 = end - mid;

    // copy right run
    std::vector<T> right(arr + mid + 1, arr + end + 1);

    ptrdiff_t i = mid;       // index in left
    ptrdiff_t j = n2 - 1;    // index in right
    ptrdiff_t k = end;       // merged index

    ptrdiff_t count_left = 0;
    ptrdiff_t count_right = 0;
    
    bool round = true, mode_on = false;

    while (i >= start && j >= 0) {

        if (cmp(right[j], arr[i])) {
            arr[k] = arr[i];
            k --, i --;
            count_left ++;
            count_right = 0;
            round = true;
        }
        else {
            arr[k] = right[j];
            k --, j --;
            count_left = 0;
            count_right ++;
            round = false;
        }

        while (i >= start && j >= 0 && (count_left | count_right) >= MIN_GALLOP) {
            count_left = count_right = 0;
            if (round)   count_left  = merge_hi_left(arr, right, start, mid, end, i, j, k, cmp);
            else         count_right = merge_hi_right(arr, right, start, mid, end, i, j, k, cmp);
            round = !round;
            mode_on = true;
        }

        if (mode_on) {
            mode_on = false;
            count_left = count_right = 0;
        }
    }

    while (j >= 0) {
        arr[k] = right[j];
        k --, j --;
    }
}

} // namespace sort_imp

