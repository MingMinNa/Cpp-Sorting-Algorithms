# Cpp-Sorting-Algorithms

English | <a href="README.zh-TW.md">繁體中文</a>

![C++23](https://img.shields.io/badge/C%2B%2B-23-blue) [![License](https://img.shields.io/badge/License-MIT-green)](./LICENSE)

This project aims to implement various sorting algorithms in a simple and easy-to-understand way,   
along with visualizations of their processes and correctness proofs.  

The goal is to make this repository a useful resource for learning, implementation, and reference.

## Project Structure
```
Cpp-Sorting-Algorithms/
│
├── include/    
├── src/
├── tests/
├── benchmark/
├── docs/
├── CMakeLists.txt
└── README.md
```

## Implemented Sorting

There are many sorting algorithms in the world, and not all are included here.  
Currently implemented algorithms are listed below:

### Comparison-based Sorting

<table>
<tr>
    <td><a>Bubble Sort</a></td>
    <td><a>Cocktail Sort</a></td>
    <td><a>Gnome Sort</a></td>
    <td><a>Selection Sort</a></td>
    <td><a>Insertion Sort</a></td>
</tr>
<tr>
    <td><a>Shell Sort</a></td>
    <td><a>Heap Sort</a></td>
    <td><a>Quick Sort</a></td>
    <td><a>Merge Sort</a></td>
    <td><a>Intro Sort</a></td>
</tr>
<tr>
    <td><a>Tim Sort</a></td>
    <td><a>Slow Sort</a></td>
    <td><a>Stooge Sort</a></td>
    <td><a>Comb Sort</a></td>
    <td><a>Cycle Sort</a></td>
</tr>
<tr>
    <td><a>Bitonic Sort</a></td>
    <td><a>Tournament Sort</a></td>
    <td><a>Odd-Even Sort</a></td>
    <td><a>Pancake Sort</a></td>
    <td><a>Strand Sort</a></td>
</tr>
<tr>
    <td><a>Patience Sort</a></td>
    <td><a>Adaptive Merge Sort</a></td>
    <td><a>Smooth Sort</a></td>
    <td></td>
    <td></td>
</tr>
</table>

### Non-comparison Sorting

<table>
<tr>
    <td><a>Bucket Sort</a></td>
    <td><a>Counting Sort</a></td>
    <td><a>Radix Sort</a></td>
    <td><a>Flash Sort</a></td>
</tr>
</table>

## Usage

All source code is located in the `include/` folder.  
Each sorting algorithm class provides the following interface:

```C++
// Compare must satisfy strict weak ordering
template <typename T, typename Compare = std::less<T>>
static void sort(T* arr, size_t n, Compare cmp = Compare{});
static inline bool is_stable();     // → whether it is stable
static inline bool is_comparison(); // → whether it is comparison-based
static inline bool in_place();      // → whether it is in-place
```

### Example
```C++
#include "include/bubble_sort.hpp"
#include <functional>

int main()
{
    size_t n = 5;
    int arr[] = {1, 7, 3, 2, 6};

    // "sort_imp"   is the namespace.
    // "BubbleSort" is the class.
    // "sort"       is the method.

    sort_imp::BubbleSort::sort(arr, n, std::less<int>());      // ascending
    sort_imp::BubbleSort::sort(arr, n, std::greater<int>());   // descending

    // For vector, you can use 
    // sort_imp::BubbleSort::sort(vec.data(), vec.size(), less<int>());
}
```

Full example: [`example.cpp`](./example.cpp)

### Notes
Some algorithms have specific constraints:
- Counting Sort does not support floating-point types.
- Bitonic Sort requires the sequence size to be a power of 2.

Some limitations come from the algorithm itself, while others come from implementation choices.  
Please check `docs/algos/` and the source code before using to avoid unexpected issues.