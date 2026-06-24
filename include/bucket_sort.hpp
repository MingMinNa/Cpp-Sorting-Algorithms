/**
 * @file bucket_sort.hpp
 * @brief  Class for bucket sort.
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

class BucketSort
{
    public:
        inline static const size_t MAX_NUM_BUCKETS = 1000u;
        BucketSort() = default;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, size_t n, Compare cmp = Compare{}, size_t num_buckets = 0u);
        static inline bool is_stable()      { return true;  }
        static inline bool is_comparison()  { return false; }
        static inline bool in_place()       { return false; }
};

template <typename T, typename Compare>
void BucketSort::sort(T* arr, size_t n, Compare cmp, size_t num_buckets)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    // if num_buckets is not specified, set it to sqrt(n) + 1
    if (num_buckets == 0) {
        num_buckets = std::min(MAX_NUM_BUCKETS, static_cast<size_t>(std::sqrt(n)) + 1);
    }

    auto [min_ele, max_ele] = find_min_max(arr, n);
    bool descending = cmp(max_ele, min_ele);

    std::vector<std::vector<T>> buckets(num_buckets);
    size_t range = static_cast<size_t>(max_ele - min_ele);

    auto calc_index = [&](const T &ele) -> size_t {
        size_t index = static_cast<size_t>(
            1.0 * (static_cast<size_t>(ele - min_ele)) / range * num_buckets
        );
        return (index >= num_buckets) ? (num_buckets - 1) : (index);
    };

    for (size_t i = 0; i < n; ++i) {
        size_t bucket_index = calc_index(arr[i]);
        buckets[bucket_index].push_back(arr[i]);
    }

    for (auto &bucket : buckets) {
        InsertionSort::sort(bucket.data(), bucket.size(), cmp);
    }

    ptrdiff_t start = 0, end = 0, step = 0, index = 0;

    if (!descending) start = 0              , end = num_buckets, step =  1;
    else             start = num_buckets - 1, end = -1         , step = -1;    

    for (ptrdiff_t i = start; i != end; i += step) {
        for (auto &e : buckets[i]) {
            arr[index] = e;
            ++ index;
        }
    }
}

} // namespace sort_imp

