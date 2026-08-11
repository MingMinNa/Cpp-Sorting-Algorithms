# 排序演算法 (Sortings)

<a href="Sortings.md">English</a> | 繁體中文

## 已實作排序法之比較
| Algorithm       |  Comparison | Stable | In-place | Best $T(n)$    | Average $T(n)$ | Worst $T(n)$   | Space (Worst) |
|-----------------|:-----------:|:------:|:--------:|----------------|----------------|----------------|---------------|
| Bubble          | ✓          | ✓      | ✓        | $O(n)$         | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Gnome           | ✓          | ✓      | ✓        | $O(n)$         | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Cocktail        | ✓          | ✓      | ✓        | $O(n)$         | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Selection       | ✓          | ✗      | ✓        | $O(n^2)$       | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Insertion       | ✓          | ✓      | ✓        | $O(n)$         | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Binary Insertion| ✓          | ✓      | ✓        | $O(n \log n)$  | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Shell (Pratt)   | ✓          | ✗      | ✓        | $O(n\log^2 n)$ | $O(n\log^2 n)$ | $O(n\log^2 n)$ | $O(1)$        |
| Slow Sort       | ✓          | ✗      | ✓        | $O(n^{(\lg n) / 2})$| $O(n^{(\lg n) / 2})$| $O(n^{(\lg n) / 2})$| $O(\log n)$   |
| Stooge Sort     | ✓          | ✗      | ✓        | $O(n^{2.71})$  | $O(n^{2.71})$  | $O(n^{2.71})$  | $O(\log n)$   |
| Comb Sort       | ✓          | ✗      | ✓        | $O(n \log n)$  | $O(n^2/2^p)$   | $O(n^2)$       | $O(1)$        |
| Cycle Sort      | ✓          | ✗      | ✓        | $O(n^2)$       | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Bitonic Sort    | ✓          | ✗      | ✓        | $O(n\log^2 n)$ | $O(n\log^2 n)$ | $O(n\log^2 n)$ | $O(\log n)$   |
| Odd-Even Sort   | ✓          | ✓      | ✓        | $O(n)$         | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Pancake Sort    | ✓          | ✗      | ✓        | $O(n^2)$       | $O(n^2)$       | $O(n^2)$       | $O(1)$        |
| Strand Sort     | ✓          | ✓      | ✗        | $O(n)$         | $O(n^2)$       | $O(n^2)$       | $O(n)$        |
| Tournament Sort | ✓          | ✗      | ✗        | $O(n\log n)$   | $O(n\log n)$   | $O(n\log n)$   | $O(n)$        |
| Patience Sort   | ✓          | ✗      | ✗        | $O(n\log n)$   | $O(n\log n)$   | $O(n\log n)$   | $O(n)$        |
| Merge Sort      | ✓          | ✓      | ✗        | $O(n\log n)$   | $O(n\log n)$   | $O(n\log n)$   | $O(n)$        |
| Adaptive Merge  | ✓          | ✓      | ✗        | $O(n)$         | $O(n\log n)$   | $O(n\log n)$   | $O(n)$        |
| Heap Sort       | ✓          | ✗      | ✓        | $O(n\log n)$   | $O(n\log n)$   | $O(n\log n)$   | $O(1)$        |
| Quick Sort      | ✓          | ✗      | ✓        | $O(n\log n)$   | $O(n\log n)$   | $O(n^2)$       | $O(n)$        |
| Tim Sort        | ✓          | ✓      | ✗        | $O(n)$         | $O(n\log n)$   | $O(n\log n)$   | $O(n)$        |
| Smooth Sort     | ✓          | ✗      | ✓        | $O(n)$         | $O(n\log n)$   | $O(n\log n)$   | $O(1)$        |
| Intro Sort      | ✓          | ✗      | ✓        | $O(n\log n)$   | $O(n\log n)$   | $O(n\log n)$   | $O(\log n)$   |
| Power Sort      | ✓          | ✓      | ✗        | $O(n)$         | $O(n\log n)$   | $O(n\log n)$   | $O(n)$        |
| Counting Sort   | ✗          | ✓      | ✗        | $O(n + k)$     | $O(n + k)$     | $O(n + k)$     | $O(n + k)$    |
| Radix Sort      | ✗          | ✓      | ✗        | $O(d(n + k))$  | $O(d(n + k))$  | $O(d(n + k))$  | $O(n + k)$    |
| Bucket Sort     | ✗          | ✓      | ✗        | $O(n + k)$     | $O(n + k)$     | $O(n^2)$       | $O(n + k)$    |
| Flash Sort      | ✗          | ✗      | ✗        | $O(n + k)$     | $O(n + k)$     | $O(n^2)$       | $O(k)$        |

### 備註
- 在 `Bucket Sort` 與 `Flash Sort` 中， $k$ 表示桶的數量。
- 在 `Counting Sort` 中， $k$ 表示值域大小。
- 在 `Radix Sort` 中， $d$ 表示數字位數。
- 在 Space (Worst) 欄位中，包含遞迴呼叫所需要的空間。


## 排序演算法使用說明
雖然大多數排序演算法在設計上具有通用性，但部分實作仍存在特定限制或使用建議，使用前請留意以下事項。

### Bitonic Sort
- 序列長度必須為 $2$ 的次方。
- 若輸入長度不符合條件，需自行進行填充。

### Strand Sort
- 即使提供陣列介面，內部仍會轉換為 `std::list` 進行處理。
- 因此在處理大型資料時，使用 `std::list` 可能會有較佳的實際效能表現。

### Counting Sort
- 僅適用於可枚舉且範圍有限的整數型別。
- 不支援浮點數型別。

### Smooth Sort
- 使用 `SmoothSort::sort` 時，當序列長度大於等於 李奧納多數-64 $L_{64}$ 時，可能會發生溢位問題。
- 建議使用 `SmoothSortV2::sort`，以避免該問題。

### 非比較式排序（通用說明）
- 非比較式排序在使用時通常需要依賴特定資料特性，因此應根據實際應用場景調整程式碼。
- 部分實作要求元素能夠轉換為 `std::size_t`，以作為索引使用。