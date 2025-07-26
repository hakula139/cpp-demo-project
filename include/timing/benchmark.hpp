/**
 * @file benchmark.hpp
 * @brief Modern C++ benchmarking utilities with high-resolution timing capabilities
 *
 * This module provides utilities for benchmarking and profiling code. It includes benchmarking
 * utilities that demonstrate modern C++ chrono features and RAII patterns.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <numeric>
#include <print>
#include <string_view>
#include <vector>

#include "algorithms/stl.hpp"
#include "concepts/callable_concepts.hpp"
#include "timer.hpp"

namespace cpp_features::timing {

/**
 * @brief Benchmarking utility class for performance analysis
 *
 * The BenchmarkRunner class provides utilities for running performance benchmarks on functions or
 * code blocks. It executes the provided function multiple times and collects statistical
 * information about execution times.
 *
 * Features:
 *
 * - Multiple iteration execution for statistical accuracy
 * - Automatic collection of timing statistics (min, max, average, total)
 * - Formatted result output with human-readable units (ns, μs, ms, s)
 * - Template-based function execution for flexibility
 *
 * @code
 * auto result = BenchmarkRunner::Benchmark("Sorting algorithm", []() {
 *   std::vector<int> data = GenerateRandomData(1000);
 *   std::ranges::sort(data);
 * }, 1000);
 * BenchmarkRunner::PrintResult(result);
 * @endcode
 */
class BenchmarkRunner {
 public:
  /**
   * @brief Structure containing benchmark results and statistics
   *
   * Contains comprehensive timing statistics from a benchmark run, including iteration count,
   * total time, and statistical measures.
   */
  struct BenchmarkResult {
    std::string_view name;   ///< Name of the benchmark
    std::size_t iterations;  ///< Number of iterations executed
    std::int64_t total_ns;   ///< Total execution time in nanoseconds
    std::int64_t avg_ns;     ///< Average execution time in nanoseconds
    std::int64_t min_ns;     ///< Minimum execution time in nanoseconds
    std::int64_t max_ns;     ///< Maximum execution time in nanoseconds
  };

  /**
   * @brief Run a benchmark on the provided function
   *
   * @tparam Func Callable type (function, lambda, functor) that can be called without arguments
   * @param name Descriptive name for the benchmark
   * @param func Function to benchmark
   * @param iterations Number of times to execute the function (default: 1000)
   * @return BenchmarkResult containing timing statistics
   *
   * Executes the provided function the specified number of times and collects timing statistics.
   * Each iteration is timed independently to provide accurate statistical analysis.
   *
   * @code
   * auto result = BenchmarkRunner::Benchmark("Vector operations", []() {
   *   std::vector<int> vec(1000);
   *   std::iota(vec.begin(), vec.end(), 0);
   *   std::ranges::sort(vec);
   * }, 500);
   * @endcode
   */
  template <concepts::NullaryCallable Func>
  [[nodiscard]] static auto Benchmark(std::string_view name, Func &&func,
                                      std::size_t iterations = 1000) -> BenchmarkResult {
    std::vector<std::int64_t> times;
    times.reserve(iterations);

    for (std::size_t i = 0; i < iterations; ++i) {
      times.push_back(TimeFunction(func));
    }

    auto total_ns = std::reduce(times.begin(), times.end(), 0LL);
    auto avg_ns = total_ns / static_cast<std::int64_t>(iterations);
    auto [min_ns, max_ns] = algorithms::FindMinMax(times);

    return {
        .name = name,
        .iterations = iterations,
        .total_ns = total_ns,
        .avg_ns = avg_ns,
        .min_ns = min_ns,
        .max_ns = max_ns,
    };
  }

  /**
   * @brief Print formatted benchmark results
   *
   * @param result The benchmark result to print
   *
   * Prints comprehensive benchmark statistics in a human-readable format with appropriate units and
   * formatting.
   *
   * @code
   * BenchmarkRunner::PrintResult(result);
   * // Output:
   * // Benchmark: Vector operations
   * // - Iterations: 500
   * // - Total time: 125.34ms
   * // - Average: 250.68μs
   * // - Min: 180.23μs
   * // - Max: 890.45μs
   * @endcode
   */
  static void PrintResult(const BenchmarkResult &result);
};

/**
 * @brief Profile a function with automatic output formatting
 *
 * @tparam Func Callable type (function, lambda, functor) that can be called without arguments
 * @param name Descriptive name for the profiled function
 * @param func Function to profile
 * @param iterations Number of iterations for benchmarking (default: 1)
 *
 * Profiles the provided function using either simple timing (for single iteration) or full
 * benchmarking (for multiple iterations). Automatically prints formatted results.
 *
 * @code
 * // Simple profiling
 * ProfileFunction("Database query", []() {
 *   ExecuteDatabaseQuery();
 * });
 *
 * // Benchmarking with multiple iterations
 * ProfileFunction("Sorting algorithm", []() {
 *   std::vector<int> data = GenerateData(1000);
 *   std::ranges::sort(data);
 * }, 100);
 * @endcode
 */
template <concepts::NullaryCallable Func>
void ProfileFunction(std::string_view name, Func &&func, std::size_t iterations = 1) {
  if (iterations == 1) {
    ScopedTimer timer{name};
    static_cast<void>(func());
  } else if (iterations > 1) {
    auto result = BenchmarkRunner::Benchmark(name, std::forward<Func>(func), iterations);
    BenchmarkRunner::PrintResult(result);
  } else {
    std::println("Invalid number of iterations: {}", iterations);
  }
}

}  // namespace cpp_features::timing
