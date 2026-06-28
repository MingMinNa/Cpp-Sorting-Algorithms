/**
 * @file benchmark_utils.hpp
 * @brief  Utilities for sorting algorithm benchmarks.
 * @author MingMinNa
 */

#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstddef>
#include <fstream>
#include <optional>
#include <iostream>
#include <algorithm>
#include <functional>

/* Data Generator */

namespace sort_benchmark 
{

enum class DataOrder {
    RANDOM,          // Uniformly shuffled
    SORTED,          // Ascending order
    REVERSE_SORTED,  // Descending order
    NEARLY_SORTED,   // 1% elements randomly swapped
    FEW_UNIQUE,      // Only sqrt(N) distinct values
    PARTIAL_REVERSE  // Alternating ascending/descending segments
};

inline std::string order_name(DataOrder order) 
{
    switch (order) {
        case DataOrder::RANDOM:          return "Random";
        case DataOrder::SORTED:          return "Sorted";
        case DataOrder::REVERSE_SORTED:  return "Reverse-Sorted";
        case DataOrder::NEARLY_SORTED:   return "Nearly-Sorted";
        case DataOrder::FEW_UNIQUE:      return "Few-Unique";
        case DataOrder::PARTIAL_REVERSE: return "Partial-Reverse";
    }
    return "Unknown";
}

inline std::vector<DataOrder> order_list()
{
    return {
        DataOrder::RANDOM, 
        DataOrder::SORTED, 
        DataOrder::REVERSE_SORTED, 
        DataOrder::NEARLY_SORTED, 
        DataOrder::FEW_UNIQUE,
        DataOrder::PARTIAL_REVERSE
    };
}

template <std::unsigned_integral T>
inline std::vector<T> 
generate_data(
    std::size_t n, DataOrder order, 
    std::optional<std::size_t> seed = std::nullopt
) {
    if (n == 0) return {};

    static std::random_device rd;
    std::mt19937 rng(seed.has_value() ? seed.value() : rd());
    std::vector<T> data(n);

    switch (order) {
        case DataOrder::RANDOM: 
        {
            std::iota(data.begin(), data.end(), 0u);
            std::shuffle(data.begin(), data.end(), rng);
            break;
        }

        // leave two elements unsorted
        case DataOrder::SORTED: 
        {
            std::iota(data.begin(), data.end(), 0u); 
            data.back() = 0u;
            data.front() = n;
            break;
        }

        // leave two elements unsorted
        case DataOrder::REVERSE_SORTED: 
        {
            std::iota(data.begin(), data.end(), 0u);
            data.back() = 0u;
            data.front() = n;
            std::reverse(data.begin(), data.end());
            break;
        }

        case DataOrder::NEARLY_SORTED: 
        {
            std::iota(data.begin(), data.end(), 0u);
            std::size_t swaps = std::max<std::size_t>(1, n / 100);
            std::uniform_int_distribution<std::size_t> dist(0, n - 1);
            for (std::size_t i = 0; i < swaps; ++i) 
                std::swap(data[dist(rng)], data[dist(rng)]);
            break;
        }

        case DataOrder::FEW_UNIQUE: 
        {
            std::size_t buckets = static_cast<std::size_t>(
                std::max<std::size_t>(2, (std::size_t)std::sqrt((double)n))
            );
            std::uniform_int_distribution<T> dist(0, buckets - 1);
            for (auto& v : data) v = dist(rng);
            break;
        }

        case DataOrder::PARTIAL_REVERSE: 
        {
            std::iota(data.begin(), data.end(), 0u);
            std::size_t num_segments = static_cast<std::size_t>(
                std::max<std::size_t>(2, (std::size_t)std::sqrt((double)n))
            );
            
            std::vector<std::size_t> boundaries;
            boundaries.push_back(0);
            
            std::uniform_int_distribution<std::size_t> pos_dist(1, n - 1);
            for (std::size_t i = 0; i < num_segments - 1; ++i)
                boundaries.push_back(pos_dist(rng));
            boundaries.push_back(n);
            
            std::sort(boundaries.begin(), boundaries.end());
            boundaries.erase(
                std::unique(boundaries.begin(), boundaries.end()), 
                boundaries.end()
            );
            
            for (std::size_t i = 0; i + 1 < boundaries.size(); ++i) {
                auto begin = data.begin() + boundaries[i];
                auto end   = data.begin() + boundaries[i + 1];
                if (i & 1) std::sort(begin, end, std::greater<T>());
            }
            break;
        }
    }
    return data;
}

}

/* BenchmarkResult */

namespace sort_benchmark 
{

struct BenchmarkResult 
{
    std::string name;
    std::string data_order;
    std::size_t data_size{};
    double      time_ns{}; // Wall-clock nanoseconds

    double time_us() const noexcept { return time_ns / 1e3; }
    double time_ms() const noexcept { return time_ns / 1e6; }
    double time_s()  const noexcept { return time_ns / 1e9; }

    void print(std::ostream& os = std::cout) const
    {
        os << "┌─────────────────────────────────────────────────┐\n";
        os << "│ Algorithm   : " << std::left << std::setw(34) << name << "│\n";
        os << "│ Data Order  : " << std::left << std::setw(34) << data_order << "│\n";
        os << "│ Data Size   : " << std::left << std::setw(34) << data_size  << "│\n";
        os << "├─────────────────────────────────────────────────┤\n";

        // Time
        if (time_s() >= 1.0) {
            os << "│ Time        : " << std::left << std::setw(29)
               << (std::to_string(time_s()).substr(0,8) + " s") << "     │\n";
        }
        else if (time_ms() >= 1.0) {
            os << "│ Time        : " << std::left << std::setw(29)
               << (std::to_string(time_ms()).substr(0,8) + " ms") << "     │\n";
        }
        else {
            os << "│ Time        : " << std::left << std::setw(30)
               << (std::to_string(time_us()).substr(0,8) + " μs") << "     │\n";
        }
        os << "└─────────────────────────────────────────────────┘\n";
    }
};

template <std::unsigned_integral T, typename SortClass>
BenchmarkResult run_sort_benchmark(
    std::string name, 
    DataOrder data_order, 
    const std::vector<T> &raw_vec
) {
    BenchmarkResult result;
    result.name = name;
    result.data_order = order_name(data_order);
    result.data_size = raw_vec.size();

    auto vec = raw_vec;
    auto ref = raw_vec;
    std::sort(ref.begin(), ref.end());

    auto t0 = std::chrono::high_resolution_clock::now();
    SortClass::template sort<T>(vec.data(), vec.size());
    auto t1 = std::chrono::high_resolution_clock::now();

    result.time_ns = static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()
    );

    // Correctness Check
    if (vec != ref) {
        std::cerr << "[Error] " << name << " result is not sorted!\n";
        std::exit(EXIT_FAILURE);
    }

    return result;
}

template <std::unsigned_integral T, typename... SortClasses>
std::vector<BenchmarkResult>
run_benchmark_suite(
    const std::vector<std::size_t> &sort_sizes, 
    std::optional<std::size_t> seed,
    std::size_t repeat = 5
) {
    std::vector<BenchmarkResult> results;
    for (DataOrder order : order_list()) {

        for (auto &size : sort_sizes) {
            ([&]<typename SortClass>() {

                BenchmarkResult avg{};

                for (std::size_t i = 0; i < repeat; i++) {
                    auto vec = generate_data<T>(size, order, seed.has_value() ? seed.value() + i : seed);
                    auto res = run_sort_benchmark<T, SortClass>(SortClass::name, order, vec);
                    avg.time_ns += res.time_ns;
                    avg.name       = res.name;
                    avg.data_order = res.data_order;
                    avg.data_size  = res.data_size;
                }

                avg.time_ns /= static_cast<double>(repeat);
                results.push_back(avg);

            }.template operator()<SortClasses>(), ...);
        }

        std::cout << "[Complete] " << order_name(order) << std::endl;
    }
    return results;
}

void to_csv(
    const std::vector<BenchmarkResult> &results, 
    std::string dest, std::string name
) {
    std::string filename = dest + "/" + (name + ".csv");
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "[Error] Cannot open file: " << filename << "\n";
        std::exit(EXIT_FAILURE);
    }

    // Columns
    file << "Algo,DataOrder,Size,Time(ns)\n";
    for (auto &res : results) {
        file << res.name        << ","
             << res.data_order  << ","
             << res.data_size   << ","
             << res.time_ns     << "\n";
    }

    file.close();
    std::cout << "[Output] Saved to " << filename << "\n";
}

}
