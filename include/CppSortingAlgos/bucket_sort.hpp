/**
 * @file bucket_sort.hpp
 * @brief  Class for bucket sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include "insertion_sort.hpp"
#include <span>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_algo 
{

class BucketSort
{
    public:
        inline static const std::string name = "Bucket Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = false;
        inline static const bool in_place      = false;

        inline static const std::size_t MAX_NUM_BUCKETS = 1000u;

        template <typename T, typename Compare = std::less<T>>
        static void sort(
            std::span<T> arr_span, 
            Compare cmp = Compare{}, 
            std::size_t num_buckets = 0u
        );

    private:
        BucketSort() = default;
};

template <typename T, typename Compare>
void BucketSort::sort(std::span<T> arr_span, Compare cmp, std::size_t num_buckets)
{
    T* arr = arr_span.data();
    std::size_t n = arr_span.size();
    
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    // if num_buckets is not specified, set it to sqrt(n) + 1
    if (num_buckets == 0) {
        num_buckets = std::min(
            MAX_NUM_BUCKETS, 
            static_cast<std::size_t>(std::sqrt(n)) + 1
        );
    }

    auto [min_ele, max_ele] = find_min_max(arr, n);
    bool descending = cmp(max_ele, min_ele);

    std::vector<std::vector<T>> buckets(num_buckets);
    std::size_t range = static_cast<std::size_t>(max_ele - min_ele);

    auto calc_index = [&](const T &ele) -> std::size_t {
        std::size_t index = static_cast<std::size_t>(
            1.0 * (static_cast<std::size_t>(ele - min_ele)) / range * num_buckets
        );
        return (index >= num_buckets) ? (num_buckets - 1) : (index);
    };

    for (std::size_t i = 0; i < n; ++i) {
        std::size_t bucket_index = calc_index(arr[i]);
        buckets[bucket_index].push_back(arr[i]);
    }

    for (auto &bucket : buckets) {
        InsertionSort::sort<T>(bucket, cmp);
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

} // namespace sort_algo

