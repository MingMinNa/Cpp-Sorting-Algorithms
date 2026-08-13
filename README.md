# Cpp-Sorting-Algorithms

English | <a href="README.zh-TW.md">繁體中文</a>

![C++23](https://img.shields.io/badge/C%2B%2B-23-blue) [![License](https://img.shields.io/badge/License-MIT-green)](./LICENSE)

This project aims to implement various sorting algorithms in a simple and easy-to-understand way.  
The goal is to make this repository a useful resource for learning, implementation, and reference.

## Project Structure
```
Cpp-Sorting-Algorithms/
│
├── include/CppSortingAlgos/
├── tests/
├── benchmarks/
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
    <td><a>Insertion Sort</a></td>
    <td><a>Binary Insertion Sort</a></td>
</tr>
<tr>
    <td><a>Selection Sort</a></td>
    <td><a>Shell Sort</a></td>
    <td><a>Heap Sort</a></td>
    <td><a>Quick Sort</a></td>
    <td><a>Merge Sort</a></td>
</tr>
<tr>
    <td><a>Intro Sort</a></td>
    <td><a>Tim Sort</a></td>
    <td><a>Slow Sort</a></td>
    <td><a>Stooge Sort</a></td>
    <td><a>Comb Sort</a></td>
</tr>
<tr>
    <td><a>Cycle Sort</a></td>
    <td><a>Bitonic Sort</a></td>
    <td><a>Tournament Sort</a></td>
    <td><a>Odd-Even Sort</a></td>
    <td><a>Pancake Sort</a></td>
</tr>
<tr>
    <td><a>Strand Sort</a></td>
    <td><a>Patience Sort</a></td>
    <td><a>Adaptive Merge Sort</a></td>
    <td><a>Smooth Sort</a></td>
    <td><a>Power Sort</a></td>
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

All source code is located in the `include/CppSortingAlgos/` folder.  
Each sorting algorithm class provides the following interface:

```C++
// Compare must satisfy strict weak ordering
template <typename T, typename Compare = std::less<T>>
static void sort(T* arr, size_t n, Compare cmp = Compare{});
inline static const bool is_stable;     // → whether it is stable
inline static const bool is_comparison; // → whether it is comparison-based
inline static const bool in_place;      // → whether it is in-place
```

### Example
```C++
#include "include/CppSortingAlgos/bubble_sort.hpp"
#include <functional>

int main()
{
    size_t n = 5;
    int arr[] = {1, 7, 3, 2, 6};

    // "sort_algo"  is the namespace.
    // "BubbleSort" is the class.
    // "sort"       is the method.

    sort_algo::BubbleSort::sort(arr, n, std::less<int>());      // ascending
    sort_algo::BubbleSort::sort(arr, n, std::greater<int>());   // descending

    // For vector, you can use 
    // sort_algo::BubbleSort::sort(vec.data(), vec.size(), less<int>());
}
```

Full example: [`example.cpp`](./example.cpp)

### Notes
Some algorithms have specific constraints, for example:
- Counting Sort does not support floating-point types.
- Bitonic Sort requires the sequence size to be a power of $2$.

Some limitations come from the algorithm itself, while others come from implementation choices.  
Please check [`docs/Sortings.md`](docs/Sortings.md) and the source code before using to avoid unexpected issues.

## Installation
To use this library in your own CMake project, you can add it with CMake's `FetchContent` module.  
Add the following to your `CMakeLists.txt`:  
```cmake
include(FetchContent)

FetchContent_Declare(
    CppSortingAlgos
    GIT_REPOSITORY https://github.com/MingMinNa/Cpp-Sorting-Algorithms.git
    GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(CppSortingAlgos)

add_executable(<target_name>
    <source_file>.cpp
    ...
)

target_link_libraries(<target_name>
    PRIVATE
        CppSortingAlgos::sort_algo
)
```

You can then include the library headers in your source file as follows:
```C++
#include <CppSortingAlgos/bubble_sort.hpp>
#include <CppSortingAlgos/insertion_sort.hpp>
#include <CppSortingAlgos/merge_sort.hpp>
...
```

If you want to modify the code, download the source and copy `include/CppSortingAlgos` folder into your project.