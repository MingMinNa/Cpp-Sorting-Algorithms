/**
 * @file strand_sort.hpp
 * @brief  Class for strand sort.
 * @author MingMinNa
 */

#pragma once

#include "sort_utils.hpp"
#include <list>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace sort_imp 
{

class StrandSort
{
    public:
        inline static const std::string name = "Strand Sort";
        inline static const bool is_stable     = true;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = false;

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});
        template <typename T, typename Compare = std::less<T>>
        static void sort(std::list<T> &list, Compare cmp = Compare{});

    private:
        StrandSort() = default;

        template <typename T, typename Compare>
        static void strand_sort(
            std::list<T> &list, 
            std::list<T> &sorted_list, 
            Compare cmp
        );
};

template <typename T, typename Compare>
void StrandSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;

    std::list<T> list(arr, arr + n), sorted_list;
    strand_sort(list, sorted_list, cmp);

    std::size_t index = 0;
    for (auto & ele : sorted_list) {
        arr[index ++] = ele;
    }
}

template <typename T, typename Compare>
void StrandSort::sort(std::list<T> &list, Compare cmp)
{
    if (list.empty()) return;
    std::list<T> sorted_list;
    strand_sort(list, sorted_list, cmp);
    list = std::move(sorted_list);
}

template <typename T, typename Compare>
void StrandSort::strand_sort(
    std::list<T> &list, 
    std::list<T> &sorted_list, 
    Compare cmp
) {
    while (!list.empty()) {
        std::list<T> sublist;
        sublist.push_back(list.front());
        list.pop_front();

        for (auto it = list.begin(); it != list.end();) {
            if (!cmp(*it, sublist.back())) {
                sublist.push_back(*it);
                it = list.erase(it);
            }
            else {
                ++it;
            }
        }

        sorted_list.merge(sublist, cmp);
    }
}

} // namespace sort_imp

