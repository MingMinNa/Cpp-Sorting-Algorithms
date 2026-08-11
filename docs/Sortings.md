# Sortings

English | <a href="Sortings.zh-TW.md">繁體中文</a>

## Comparison of Implemented Sortings
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

### Notes
- In `Bucket Sort` and `Flash Sort`, $k$ denotes the number of buckets.
- In `Counting Sort`, $k$ denotes the size of the value range.
- In `Radix Sort`, $d$ denotes the number of digits.
- In the Space (Worst) column, the space required for recursive calls is included.


## Usage Notes

Although most sorting algorithms are general-purpose, some have specific constraints or usage notes.

### Bitonic Sort
- Sequence length must be a power of $2$.
- Otherwise, padding or preprocessing is required.

### Strand Sort
- Internally uses `std::list` even if an array interface is provided.
- `std::list` may perform better for large inputs.

### Counting Sort
- Only works for integer types with a limited range.
- Floating-point types are not supported.

### Smooth Sort
- `SmoothSort::sort` may overflow when the sizes ≥ $L_{64}$.
- Use `SmoothSortV2::sort` instead for safety.

### Non-comparison Sorting (General)
- Depends on specific data properties, so modify the code as necessary.
- Some implementations require conversion to `std::size_t` for indexing.