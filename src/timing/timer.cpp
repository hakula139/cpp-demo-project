#include "timing/timer.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>
#include <print>
#include <thread>
#include <vector>

namespace cpp_features::timing {

void DemonstrateTimerUsage() {
  std::print("\n=== Timer Usage Demonstration ===\n");

  Timer timer;
  std::print("Timer started...\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  timer.Stop();
  std::print("Timer stopped after {}\n", timer.GetElapsedString());

  std::print("Elapsed nanoseconds: {}\n", timer.GetElapsed<std::chrono::nanoseconds>());
  std::print("Elapsed microseconds: {}\n", timer.GetElapsed<std::chrono::microseconds>());
  std::print("Elapsed milliseconds: {}\n", timer.GetElapsed<std::chrono::milliseconds>());

  timer.Reset();
  std::print("Timer reset\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(25));
  std::print("New elapsed time: {}\n", timer.GetElapsedString());
}

void DemonstrateScopedTimer() {
  std::print("\n=== Scoped Timer Demonstration ===\n");

  {
    ScopedTimer timer("Scoped operation");
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    std::print("Doing some work inside scoped timer...\n");
  }

  {
    ScopedTimer timer("With callback",
                      [](long long ns) { std::print("Callback received: {}ns\n", ns); });

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    std::print("Work with callback...\n");
  }
}

void DemonstrateBenchmarking() {
  std::print("\n=== Benchmarking Demonstration ===\n");

  auto simple_task = []() {
    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) {
      sum += i;
    }
  };

  auto result = BenchmarkRunner::Benchmark("Simple Loop", simple_task, 1000);
  BenchmarkRunner::PrintResult(result);

  auto vector_ops = []() {
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::sort(vec.begin(), vec.end(), std::greater<int>());
  };

  auto vector_result = BenchmarkRunner::Benchmark("Vector Operations", vector_ops, 500);
  BenchmarkRunner::PrintResult(vector_result);
}

void DemonstrateChronoFeatures() {
  std::print("\n=== Chrono Features Demonstration ===\n");

  [[maybe_unused]] auto now = Clock::now();
  std::print("Current time point captured\n");

  auto start = Clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  auto end = Clock::now();

  auto duration = end - start;
  std::print("Duration type measurement: {}ns\n",
             std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());

  auto time_func_result =
      TimeFunction([]() { std::this_thread::sleep_for(std::chrono::microseconds(500)); });
  std::print("TimeFunction result: {}ns\n", time_func_result);

  ProfileFunction("Profile test", []() {
    volatile double sum = 0.0;
    for (int i = 0; i < 10000; ++i) {
      sum += std::sqrt(i);
    }
  });

  ProfileFunction(
      "Profile benchmark",
      []() {
        std::vector<int> data(1000);
        std::iota(data.begin(), data.end(), 1);
        std::reverse(data.begin(), data.end());
      },
      100);
}

}  // namespace cpp_features::timing
