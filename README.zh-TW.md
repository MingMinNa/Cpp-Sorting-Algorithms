# Cpp-Sorting-Algorithms

<a href="README.md">English</a> | 繁體中文

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue) [![License](https://img.shields.io/badge/License-MIT-green)](./LICENSE)

本專案旨在以簡單且易於理解的方式實作各種排序演算法。  
期望能將本專案發展成兼具實作、學習與參考價值的排序演算法資源。

## 專案架構
```
Cpp-Sorting-Algorithms/
│
├── include/CppSortingAlgos/
├── tests/
├── benchmarks/
├── docs/
├── CMakeLists.txt
└── README.zh-TW.md
```

## 已實作的排序演算法

排序演算法千千萬萬，實在難以全數囊括，目前已實作的演算法如下：

### 比較排序法

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

### 非比較排序法

<table>
<tr>
    <td><a>Bucket Sort</a></td>
    <td><a>Counting Sort</a></td>
    <td><a>Radix Sort</a></td>
    <td><a>Flash Sort</a></td>
</tr>
</table>

## 使用方法

原始程式碼位於 `include/CppSortingAlgos/` 資料夾內，每個排序演算法類別均提供以下的方法。

```C++
// Compare 必須滿足嚴格弱序
template <typename T, typename Compare = std::less<T>>
static void sort(T* arr, size_t n, Compare cmp = Compare{});
inline static const bool is_stable;     // → 是否為穩定排序
inline static const bool is_comparison; // → 是否為比較排序
inline static const bool in_place;      // → 是否為原地排序
```

以下為使用範例
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

完整範例請看 [`example.cpp`](./example.cpp)。

此外，有些排序法會有各自的限制，例如：
- Counting Sort 不適用於浮點數資料型態上。
- Bitonic Sort 要求序列長度必須是 $2$ 的次方。

有些是演算法本身的限制，而有些是實作上的考量。  
在使用前，請先參照 [`docs/Sortings.zh-TW.md`](docs/Sortings.zh-TW.md) 中的檔案與原始碼，以免踩坑。

## 安裝
若要在你自己的 CMake 專案中使用這個函式庫，可以透過 CMake 的 `FetchContent` 模組加入。  
請在你的 `CMakeLists.txt` 中加入以下內容：  
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

接著，你就可以在自己的專案中使用它了。
```C++
#include <CppSortingAlgos/bubble_sort.hpp>
#include <CppSortingAlgos/insertion_sort.hpp>
#include <CppSortingAlgos/merge_sort.hpp>
...
```

如果你想修改程式碼，可以下載原始碼，並把 `include/CppSortingAlgos` 資料夾複製到你的專案中。