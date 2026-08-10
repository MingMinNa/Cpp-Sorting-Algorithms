# Testing

This section describes how to build and run the test suite for sorting algorithms.

## Requirements
- CMake ≥ 3.20
- C++23 compatible compiler

You can verify your CMake version with:
```bash
$ cmake --version
# cmake X.X.X
```

## Build Instructions
Configure and build the project using CMake:
```bash
$ cmake -S . -B build
$ cmake --build build
```

## Run Tests
Tests are executed using CTest:
```bash
$ ctest --test-dir build
```

## Example Output
A successful test run will produce output similar to the following:
```text
     Start  1: adaptive_merge_test
 1/29 Test  #1: adaptive_merge_test ..............   Passed    0.06 sec
      Start  2: binary_insertion_test
 2/29 Test  #2: binary_insertion_test ............   Passed    0.06 sec
      Start  3: bitonic_test
 3/29 Test  #3: bitonic_test .....................   Passed    0.03 sec
      Start  4: bubble_test

...

27/29 Test #27: strand_test ......................   Passed    0.20 sec
      Start 28: tim_test
28/29 Test #28: tim_test .........................   Passed    0.04 sec
      Start 29: tournament_test
29/29 Test #29: tournament_test ..................   Passed    0.04 sec

100% tests passed, 0 tests failed out of 29

Total Test time (real) =   6.86 sec
```