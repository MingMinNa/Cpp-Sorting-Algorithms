/**
 * @file bench_fast.cpp
 * @brief  Benchmark driver for fast comparison-based sorts.
 * @author MingMinNa
 */

#include "../include/CppSortingAlgos/sort_collection.hpp"
#include "benchmark_utils.hpp"

#include <cstring>
#include <string>
#include <vector>
#include <optional>
#include <iostream>

using namespace sort_algo;
using namespace sort_benchmark;

std::optional<std::size_t> seed = std::nullopt;
std::size_t repeat = 5;
std::size_t start_n = 16;
std::size_t max_n = (1u << 20);

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];

        if (arg == "--seed") {
            if (i + 1 >= argc) {
                std::cerr << "[Error]: --seed requires an integer argument.\n";
                std::exit(EXIT_FAILURE);
            }

            std::size_t value = std::stoi(argv[i + 1]);
            seed = value;
            break;
        }
    }

    std::cout << "Seed: " << (seed.has_value() ? std::to_string(seed.value()) : "random") << std::endl; 

    std::vector<std::size_t> sort_sizes = {start_n};
    for (;(sort_sizes.back() << 1) <= max_n; ) {
        sort_sizes.push_back(sort_sizes.back() << 1);
    }

    auto results = run_benchmark_suite<
        std::uint64_t,      // The first type is datatype (only std::unsigned_integral)
        HeapSort,            QuickSort,     MergeSort,      AdaptiveMergeSort,  TournamentSort,
        IntroSort,           TimSort,       SmoothSort,     SmoothSortV2,       PatienceSort,
        BitonicSort,         PowerSort
    >(sort_sizes, seed, repeat);
    to_csv(results, "benchmarks/results", "fast-comparison");
    
    return 0;
}