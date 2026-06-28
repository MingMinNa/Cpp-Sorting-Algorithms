/**
 * @file bench_non_comp.cpp
 * @brief  Benchmark driver for non-comparison sorts.
 * @author MingMinNa
 */

#include "../include/sort_collection.hpp"
#include "benchmark_utils.hpp"

#include <cstring>
#include <string>
#include <vector>
#include <optional>
#include <iostream>

using namespace sort_imp;
using namespace sort_benchmark;

std::optional<std::size_t> seed = std::nullopt;
std::size_t repeat = 3;
std::size_t start_n = 16;
std::size_t max_n = (1u << 16);

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
        std::uint64_t,
        BucketSort, CountingSort, 
        RadixSort,  FlashSort
    >(sort_sizes, seed, repeat);
    to_csv(results, "benchmarks/results", "non-comparison");

    return 0;
}
