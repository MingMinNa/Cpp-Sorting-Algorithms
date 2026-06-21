/**
 * @file sort_utils.hpp
 * @brief  Common utilities for generic sorting algorithms.
 * @author MingMinNa
 */

#pragma once

#include <random>
#include <cstddef>
#include <utility>
#include <stdexcept>

// 1-based array index ↔︎ 0-based array index
#define TO_ZERO_BASE(x) ((x) - 1)
#define TO_ONE_BASE(x)  ((x) + 1)

namespace sort_imp
{

size_t random_generate(size_t lower_bound, size_t upper_bound) 
{
    if (lower_bound > upper_bound)
        throw std::invalid_argument(
            "Lower bound must not exceed upper bound."
        );

    static std::mt19937_64 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(lower_bound, upper_bound);
    return dist(gen);
}

template <typename T>
void check_array_valid(const T* arr, size_t n) 
{
    if (!arr && n > 0) {
        throw std::invalid_argument(
            "The array is nullptr but n > 0"
        );
    }
}

template <typename T, typename Compare>
bool check_sorted(T* arr, size_t n, Compare cmp)
{
    check_array_valid<T>(arr, n);
    for (size_t i = 1; i < n; ++i) {
        if (cmp(arr[i], arr[i - 1])) {
            return false;
        }
    }
    return true;
}

template <typename T>
std::pair<T, T> find_min_max(const T* arr, size_t n)
{
    if (!arr || n == 0) 
        throw std::invalid_argument(
            "The array cannot be empty"
        );

    size_t i = 1;
    T min_ele = arr[0], max_ele = arr[0];
    T smaller = min_ele, larger  = max_ele;

    if (n % 2 == 0) {
        max_ele = (arr[0] > arr[1]) ? (arr[0]) : (arr[1]);
        min_ele = (arr[0] > arr[1]) ? (arr[1]) : (arr[0]);
        i = 2;
    }

    for (; i < n; i += 2) {
        if (arr[i] < arr[i + 1]) {
            larger  = arr[i + 1];
            smaller = arr[i];
        }
        else {
            larger  = arr[i];
            smaller = arr[i + 1];
        }
        if (larger  > max_ele) max_ele = larger;
        if (smaller < min_ele) min_ele = smaller;
    }

    return {min_ele, max_ele};
}

} // namespace sort_imp