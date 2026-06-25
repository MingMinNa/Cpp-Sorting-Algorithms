/**
 * @file smooth_sort.hpp
 * @brief  Class for smooth sort.
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

/* ————————————— Smooth Sort Version 1 ————————————— */

namespace sort_imp 
{

class SmoothSort
{
    public:
        inline static const std::string name = "Smooth Sort";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;
        
        inline static const std::vector<std::size_t> LEONARDO_SEQUENCE = {
            1ull,	1ull,	3ull,	5ull,	9ull,	15ull,	25ull,	41ull,	67ull,	109ull,	
            177ull,	287ull,	465ull,	753ull,	1219ull,	1973ull,	3193ull,	5167ull,	8361ull,	13529ull,	
            21891ull,	35421ull,	57313ull,	92735ull,	150049ull,	242785ull,	392835ull,	635621ull,	1028457ull,	1664079ull,	
            2692537ull,	4356617ull,	7049155ull,	11405773ull,	18454929ull,	29860703ull,	48315633ull,	78176337ull,	126491971ull,	204668309ull,	
            331160281ull,	535828591ull,	866988873ull,	1402817465ull,	2269806339ull,	3672623805ull,	5942430145ull,	9615053951ull,	15557484097ull,	25172538049ull,	
            40730022147ull,	65902560197ull,	106632582345ull,	172535142543ull,	279167724889ull,	451702867433ull,	730870592323ull,	1182573459757ull,	1913444052081ull,	3096017511839ull,	
            5009461563921ull,	8105479075761ull,	13114940639683ull,	21220419715445ull,	34335360355129ull,	55555780070575ull,	89891140425705ull,	145446920496281ull,	235338060921987ull,	380784981418269ull,	
            616123042340257ull,	996908023758527ull,	1613031066098785ull,	2609939089857313ull,	4222970155956099ull,	6832909245813413ull,	11055879401769513ull,	17888788647582927ull,	28944668049352441ull,	46833456696935369ull,	
            75778124746287811ull,	122611581443223181ull,	198389706189510993ull,	321001287632734175ull,	519390993822245169ull,	840392281454979345ull,	1359783275277224515ull,	2200175556732203861ull,	3559958832009428377ull,	5760134388741632239ull,	
            9320093220751060617ull,	15080227609492692857ull
        };

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        SmoothSort() = default;
        
        // The rightmost (smallest) order currently in the forest.
        static inline std::size_t rightmost_order(uint64_t bits)
        {
            return static_cast<std::size_t>(std::countr_zero(bits));
        }
    
        // The second-rightmost order (next set bit after the lowest).
        static inline std::size_t second_rightmost_order(uint64_t bits)
        {
            // clear lowest set bit
            uint64_t without_lowest = bits & (bits - 1);   
            return static_cast<std::size_t>(std::countr_zero(without_lowest));
        }

        template <typename T, typename Compare>
        static void sift(T* arr, std::size_t root_pos, std::size_t order, Compare cmp);
    
        template <typename T, typename Compare>
        static void trinkle(
            T* arr, std::size_t root_pos, std::size_t order,
            uint64_t heap_bits, Compare cmp
        );
};

template <typename T, typename Compare>
void SmoothSort::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
 
    /* Phase 1 – Build the Leonardo heap forest. */

    uint64_t heap_bits = 0;
 
    for (std::size_t i = 0; i < n; ++i) {
 
        if (heap_bits != 0) {
            
            std::size_t k = rightmost_order(heap_bits);
            std::size_t k2 = second_rightmost_order(heap_bits);
 
            if (std::popcount(heap_bits) >= 2) {
                // Leonardo: L(k + 2) = L(k + 1) + L(k) + 1
                if (k2 == k + 1) {
                    heap_bits &= ~(1ULL << k);        // remove order k
                    heap_bits &= ~(1ULL << k2);       // remove order k+1
                    heap_bits |=  (1ULL << (k2 + 1)); // add order k+2
                    goto do_trinkle;
                }
            }
 
            // No consecutive pair at the right end.
            if (k == 1)  heap_bits |= (1ULL << 0);
            else         heap_bits |= (1ULL << 1);
        } 
        else {
            heap_bits |= (1ULL << 1);
        }
 
        do_trinkle:
        trinkle(arr, i, rightmost_order(heap_bits), heap_bits, cmp);
    }

    /* Phase 2 – Sorted extraction. */
 
    for (std::size_t i = n - 1; i > 0; --i) {

        std::size_t order = rightmost_order(heap_bits);
        heap_bits &= heap_bits - 1;
 
        if (order >= 2) {
            std::size_t right_child = i - 1;
            std::size_t left_child  = i - 1 - LEONARDO_SEQUENCE[order - 2];
 
            // Expose left child (order-1) and right child (order-2).
            heap_bits |= (1ULL << (order - 1));
            trinkle(arr, left_child, order - 1, heap_bits, cmp);
 
            heap_bits |= (1ULL << (order - 2));
            trinkle(arr, right_child, order - 2, heap_bits, cmp);
        }
    }
}
 
template <typename T, typename Compare>
void SmoothSort::sift(T* arr, std::size_t root_pos, std::size_t order, Compare cmp)
{
    // Orders 0 and 1 are single-element heaps — nothing to sift.

    while (order >= 2) {

        std::size_t right_child = root_pos - 1;
        std::size_t left_child  = root_pos - 1 - LEONARDO_SEQUENCE[order - 2];
 
        // Find the larger of the two children.
        std::size_t larger_child;
        std::size_t child_order;
        
        if (cmp(arr[left_child], arr[right_child])) {
            larger_child = right_child;
            child_order  = order - 2;
        }
        else {
            larger_child = left_child;
            child_order  = order - 1;
        }
 
        // If the root is already >= the larger child, the heap is valid.
        if (!cmp(arr[root_pos], arr[larger_child])) break;
 
        std::swap(arr[root_pos], arr[larger_child]);
        root_pos = larger_child;
        order    = child_order;
    }
}
 
template <typename T, typename Compare>
void SmoothSort::trinkle(
    T* arr, std::size_t root_pos, std::size_t order,
    uint64_t heap_bits, Compare cmp
) {
    std::size_t current_pos   = root_pos;
    std::size_t current_order = order;
 
    while (true) {

        // Remove the current tree's bit;
        uint64_t remaining = heap_bits & ~(1ULL << current_order);
        if (remaining == 0) break;
 
        std::size_t left_order = rightmost_order(remaining);
        std::size_t left_root  = current_pos - LEONARDO_SEQUENCE[current_order];
 
        if (!cmp(arr[current_pos], arr[left_root])) break;
 
        if (current_order >= 2) {
            std::size_t right_child = current_pos - 1;
            std::size_t left_child  = current_pos - 1 - LEONARDO_SEQUENCE[current_order - 2];
 
            if (cmp(arr[left_root], arr[right_child]) ||
                cmp(arr[left_root], arr[left_child])
            ) break; // A child is larger — sift the current root down instead.
        }
 
        std::swap(arr[current_pos], arr[left_root]);
        heap_bits    = remaining;
        current_pos  = left_root;
        current_order = left_order;
    }
 
    sift(arr, current_pos, current_order, cmp);
}

} // namespace sort_imp

/* ————————————— Smooth Sort Version 2 ————————————— */

namespace sort_imp 
{

class SmoothSortV2
{
    public:
        inline static const std::string name = "Smooth Sort V2";
        inline static const bool is_stable     = false;
        inline static const bool is_comparison = true;
        inline static const bool in_place      = true;

        inline static const std::vector<std::size_t> LEONARDO_SEQUENCE = {
            1ull,	1ull,	3ull,	5ull,	9ull,	15ull,	25ull,	41ull,	67ull,	109ull,	
            177ull,	287ull,	465ull,	753ull,	1219ull,	1973ull,	3193ull,	5167ull,	8361ull,	13529ull,	
            21891ull,	35421ull,	57313ull,	92735ull,	150049ull,	242785ull,	392835ull,	635621ull,	1028457ull,	1664079ull,	
            2692537ull,	4356617ull,	7049155ull,	11405773ull,	18454929ull,	29860703ull,	48315633ull,	78176337ull,	126491971ull,	204668309ull,	
            331160281ull,	535828591ull,	866988873ull,	1402817465ull,	2269806339ull,	3672623805ull,	5942430145ull,	9615053951ull,	15557484097ull,	25172538049ull,	
            40730022147ull,	65902560197ull,	106632582345ull,	172535142543ull,	279167724889ull,	451702867433ull,	730870592323ull,	1182573459757ull,	1913444052081ull,	3096017511839ull,	
            5009461563921ull,	8105479075761ull,	13114940639683ull,	21220419715445ull,	34335360355129ull,	55555780070575ull,	89891140425705ull,	145446920496281ull,	235338060921987ull,	380784981418269ull,	
            616123042340257ull,	996908023758527ull,	1613031066098785ull,	2609939089857313ull,	4222970155956099ull,	6832909245813413ull,	11055879401769513ull,	17888788647582927ull,	28944668049352441ull,	46833456696935369ull,	
            75778124746287811ull,	122611581443223181ull,	198389706189510993ull,	321001287632734175ull,	519390993822245169ull,	840392281454979345ull,	1359783275277224515ull,	2200175556732203861ull,	3559958832009428377ull,	5760134388741632239ull,	
            9320093220751060617ull,	15080227609492692857ull
        };

        template <typename T, typename Compare = std::less<T>>
        static void sort(T* arr, std::size_t n, Compare cmp = Compare{});

    private:
        SmoothSortV2() = default;
        
        template <typename T, typename Compare>
        static void sift(T* arr, std::size_t root_pos, std::size_t order, Compare cmp);
 
        template <typename T, typename Compare>
        static void trinkle(
            T* arr, std::size_t root_pos, std::size_t order,
            const std::vector<std::size_t>& heap_sizes, Compare cmp
        );
};

template <typename T, typename Compare>
void SmoothSortV2::sort(T* arr, std::size_t n, Compare cmp)
{
    if (check_sorted<T, Compare>(arr, n, cmp)) return;
 
    /* Phase 1 – Build the Leonardo heap forest. */

    // Leonardo orders of each tree, L → R
    std::vector<std::size_t> heap_sizes;  
    heap_sizes.reserve(64);
 
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t size = heap_sizes.size();
 
        if (size >= 2 && heap_sizes[size - 2] == heap_sizes[size - 1] + 1) {
            // Leonardo: L(k + 2) = L(k + 1) + L(k) + 1
            heap_sizes.pop_back();
            heap_sizes.back() += 1;
        }
        else if (size >= 1 && heap_sizes.back() == 1) {
            heap_sizes.push_back(0);
        }
        else {
            heap_sizes.push_back(1);
        }
        
        trinkle(arr, i, heap_sizes.back(), heap_sizes, cmp);
    }
 
    /* Phase 2 – Sorted extraction. */

    for (std::size_t i = n - 1; i > 0; --i) {

        std::size_t order = heap_sizes.back();
        heap_sizes.pop_back();
 
        if (order >= 2) {
            std::size_t right_child = i - 1;
            std::size_t left_child  = i - 1 - LEONARDO_SEQUENCE[order - 2];
 
            // Push left child's tree (order-1), then right child's tree (order-2).
            heap_sizes.push_back(order - 1);
            trinkle(arr, left_child, order - 1,heap_sizes, cmp);
 
            heap_sizes.push_back(order - 2);
            trinkle(arr, right_child, order - 2, heap_sizes, cmp);
        }
    }
}

template <typename T, typename Compare>
void SmoothSortV2::sift(T* arr, std::size_t root_pos, std::size_t order, Compare cmp)
{
    // Orders 0 and 1 are single-element heaps — nothing to sift.

    while (order >= 2) {

        std::size_t right_child = root_pos - 1;
        std::size_t left_child  = root_pos - 1 - LEONARDO_SEQUENCE[order - 2];
 
        // Find the larger of the two children.
        std::size_t larger_child;
        std::size_t child_order;
        
        if (cmp(arr[left_child], arr[right_child])) {
            larger_child = right_child;
            child_order  = order - 2;
        }
        else {
            larger_child = left_child;
            child_order  = order - 1;
        }
 
        // If the root is already >= the larger child, the heap is valid.
        if (!cmp(arr[root_pos], arr[larger_child])) break;
 
        std::swap(arr[root_pos], arr[larger_child]);
        root_pos = larger_child;
        order    = child_order;
    }
}

template <typename T, typename Compare>
void SmoothSortV2::trinkle(
    T* arr, std::size_t root_pos, std::size_t order,
    const std::vector<std::size_t>& heap_sizes, Compare cmp
) {
    std::size_t current_pos   = root_pos;
    std::size_t current_order = order;
    std::size_t heap_idx = heap_sizes.size() - 1;
 
    while (heap_idx > 0) {
        // Position of the left neighbor root.
        std::size_t left_root = current_pos - LEONARDO_SEQUENCE[current_order];
 
        if (!cmp(arr[current_pos], arr[left_root])) break;
 
        if (current_order >= 2) {
            std::size_t right_child = current_pos - 1;
            std::size_t left_child  = current_pos - 1 - LEONARDO_SEQUENCE[current_order - 2];
 
            if (cmp(arr[left_root], arr[right_child]) ||
                cmp(arr[left_root], arr[left_child])
            ) break; // A child is larger — sift the current root down instead.
        }
 
        // Swap with left neighbour root and move left.
        std::swap(arr[current_pos], arr[left_root]);
        current_pos   = left_root;
        current_order = heap_sizes[heap_idx - 1];
        -- heap_idx;
    }

    sift(arr, current_pos, current_order, cmp);
}

} // namespace sort_imp

