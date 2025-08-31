/**
 * @file timing_example.cpp
 * @brief Example demonstrating the usage of the timing module
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <functional>
#include <numeric>
#include <print>
#include <thread>
#include <vector>

#include "timing/benchmark.hpp"
#include "timing/timer.hpp"

using cpp_features::timing::BenchmarkRunner;
using cpp_features::timing::ProfileFunction;
using cpp_features::timing::ScopedTimer;
using cpp_features::timing::TimeFunction;
using cpp_features::timing::Timer;

namespace {

/**
 * @brief Demonstrate basic timer usage with manual start / stop
 */
void DemonstrateBasicTimer() {
  std::println("\n1. Basic Timer Usage:");

  Timer timer;
  std::println("   Timer started...");

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  timer.Stop();
  std::println("   Timer stopped after {}", timer.GetElapsedString());
  std::println("   Elapsed nanoseconds: {}", timer.GetElapsed<std::chrono::nanoseconds>());
  std::println("   Elapsed microseconds: {}", timer.GetElapsed<std::chrono::microseconds>());
  std::println("   Elapsed milliseconds: {}", timer.GetElapsed<std::chrono::milliseconds>());

  timer.Reset();
  std::println("   Timer reset");

  std::this_thread::sleep_for(std::chrono::milliseconds(25));
  std::println("   New elapsed time: {}", timer.GetElapsedString());
}

/**
 * @brief Demonstrate RAII-based scoped timer
 */
void DemonstrateScopedTimer() {
  std::println("\n2. Scoped Timer Usage:");

  {
    ScopedTimer timer{"Scoped operation"};
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    std::println("   Doing some work inside scoped timer...");
  }

  {
    ScopedTimer timer{
        "Scoped operation with callback",
        [](std::int64_t ns) { std::println("   Callback received: {}ns", ns); },
    };
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    std::println("   Doing some work with callback...");
  }
}

/**
 * @brief Demonstrate benchmarking capabilities
 */
void DemonstrateBenchmarking() {
  std::println("\n3. Benchmarking:");

  auto simple_task = []() {
    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) {
      sum += i;
    }
  };

  auto result = BenchmarkRunner::Benchmark("Simple task", simple_task, 1000);
  BenchmarkRunner::PrintResult(result);

  auto vector_ops = []() {
    std::vector<int> vec(1000);
    // NOLINTNEXTLINE(modernize-use-ranges): Not supported yet
    std::iota(vec.begin(), vec.end(), 1);
    std::ranges::sort(vec, std::greater<>());
  };

  auto vector_result = BenchmarkRunner::Benchmark("Vector Operations", vector_ops, 500);
  BenchmarkRunner::PrintResult(vector_result);
}

/**
 * @brief Demonstrate utility functions and chrono features
 */
void DemonstrateUtilityFunctions() {
  std::println("\n4. Utility Functions:");

  // TimeFunction utility
  auto result = TimeFunction([]() { std::this_thread::sleep_for(std::chrono::microseconds(500)); });
  std::println("   TimeFunction result: {}ns", result);

  // ProfileFunction with single execution
  ProfileFunction("Profile test", []() {
    volatile double sum = 0.0;
    for (int i = 0; i < 10000; ++i) {
      sum += std::sqrt(i);
    }
  });

  // ProfileFunction with benchmarking
  ProfileFunction(
      "Profile benchmark",
      []() {
        std::vector<int> data(1000);
        // NOLINTNEXTLINE(modernize-use-ranges): Not supported yet
        std::iota(data.begin(), data.end(), 1);
        std::ranges::reverse(data);
      },
      100);
}

/**
 * @brief Demonstrate chrono features and duration types
 */
void DemonstrateChronoFeatures() {
  std::println("\n5. Chrono Features:");

  auto start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = end - start;
  std::println("   Direct duration measurement: {}ns",
               std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());

  Timer timer;
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  auto duration_obj = timer.GetElapsedDuration<std::chrono::nanoseconds>();
  std::println("   Duration object: {}ns", duration_obj.count());

  if (duration_obj > std::chrono::milliseconds(1)) {
    std::println("   Duration comparison: Operation took longer than 1ms");
  }
}

}  // namespace

auto main() -> int {
  std::println("=== Timing Module Example ===");

  DemonstrateBasicTimer();
  DemonstrateScopedTimer();
  DemonstrateBenchmarking();
  DemonstrateUtilityFunctions();
  DemonstrateChronoFeatures();

  std::println("\n=== Timing Module Example Completed ===");
  return 0;
}
