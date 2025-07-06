/**
 * @file benchmark.cpp
 * @brief Implementation of benchmarking utilities
 */

#include "timing/benchmark.hpp"

#include <print>

#include "timing/timer.hpp"

namespace cpp_features::timing {

void BenchmarkRunner::PrintResult(const BenchmarkResult &result) {
  std::println("Benchmark: {}", result.name);
  std::println("- Iterations: {}", result.iterations);
  std::println("- Total time: {}", ToHumanReadable(result.total_ns));
  std::println("- Average: {}", ToHumanReadable(result.avg_ns));
  std::println("- Min: {}", ToHumanReadable(result.min_ns));
  std::println("- Max: {}", ToHumanReadable(result.max_ns));
}

}  // namespace cpp_features::timing
