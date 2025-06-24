#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <functional>
#include <vector>
#include <print>
#include <format>
#include <numeric>
#include <algorithm>

namespace cpp_features::timing {

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

template<typename Rep, typename Period>
using TimeInterval = std::chrono::duration<Rep, Period>;

class Timer {
 public:
  Timer() : start_time_(Clock::now()) {}

  void Start() {
    start_time_ = Clock::now();
  }

  void Stop() {
    end_time_ = Clock::now();
  }

  template<typename DurationType = std::chrono::milliseconds>
  [[nodiscard]] auto GetElapsed() const -> typename DurationType::rep {
    auto end = (end_time_ == TimePoint{}) ? Clock::now() : end_time_;
    return std::chrono::duration_cast<DurationType>(end - start_time_).count();
  }

  template<typename DurationType = std::chrono::milliseconds>
  [[nodiscard]] auto GetElapsedDuration() const -> DurationType {
    auto end = (end_time_ == TimePoint{}) ? Clock::now() : end_time_;
    return std::chrono::duration_cast<DurationType>(end - start_time_);
  }

  [[nodiscard]] auto GetElapsedString() const -> std::string {
    auto elapsed_ns = GetElapsed<std::chrono::nanoseconds>();

    if (elapsed_ns < 1000) {
      return std::format("{}ns", elapsed_ns);
    } else if (elapsed_ns < 1000000) {
      return std::format("{:.2f}μs", elapsed_ns / 1000.0);
    } else if (elapsed_ns < 1000000000) {
      return std::format("{:.2f}ms", elapsed_ns / 1000000.0);
    } else {
      return std::format("{:.2f}s", elapsed_ns / 1000000000.0);
    }
  }

  void Reset() {
    start_time_ = Clock::now();
    end_time_ = TimePoint{};
  }

 private:
  TimePoint start_time_;
  TimePoint end_time_{};
};

class ScopedTimer {
 public:
  explicit ScopedTimer(std::string_view name) : name_(name), timer_() {}

  template<typename Func>
  explicit ScopedTimer(std::string_view name, Func callback)
    : name_(name), timer_(), callback_(callback) {}

  ~ScopedTimer() {
    timer_.Stop();
    auto elapsed = timer_.GetElapsedString();
    std::print("{}: {}\n", name_, elapsed);

    if (callback_) {
      callback_(timer_.GetElapsed<std::chrono::nanoseconds>());
    }
  }

 private:
  std::string name_;
  Timer timer_;
  std::function<void(long long)> callback_;
};

class BenchmarkRunner {
 public:
  struct BenchmarkResult {
    std::string name;
    std::size_t iterations;
    long long total_ns;
    long long avg_ns;
    long long min_ns;
    long long max_ns;
  };

  template<typename Func>
  static auto Benchmark(std::string_view name, Func&& func, std::size_t iterations = 1000)
    -> BenchmarkResult {
    std::vector<long long> times;
    times.reserve(iterations);

    for (std::size_t i = 0; i < iterations; ++i) {
      Timer timer;
      func();
      timer.Stop();
      times.push_back(timer.GetElapsed<std::chrono::nanoseconds>());
    }

    auto total = std::accumulate(times.begin(), times.end(), 0LL);
    auto avg = total / static_cast<long long>(iterations);
    auto min_max = std::minmax_element(times.begin(), times.end());

    return {
      std::string(name),
      iterations,
      total,
      avg,
      *min_max.first,
      *min_max.second
    };
  }

  static void PrintResult(const BenchmarkResult& result) {
    std::print("Benchmark: {}\n", result.name);
    std::print("  Iterations: {}\n", result.iterations);
    std::print("  Total time: {:.2f}ms\n", result.total_ns / 1000000.0);
    std::print("  Average: {:.2f}μs\n", result.avg_ns / 1000.0);
    std::print("  Min: {:.2f}μs\n", result.min_ns / 1000.0);
    std::print("  Max: {:.2f}μs\n", result.max_ns / 1000.0);
  }


};

void DemonstrateTimerUsage();

void DemonstrateScopedTimer();

void DemonstrateBenchmarking();

void DemonstrateChronoFeatures();

template<typename Func>
[[nodiscard]] auto TimeFunction(Func&& func) -> long long {
  Timer timer;
  func();
  timer.Stop();
  return timer.GetElapsed<std::chrono::nanoseconds>();
}

template<typename Func>
void ProfileFunction(std::string_view name, Func&& func, std::size_t iterations = 1) {
  if (iterations == 1) {
    ScopedTimer timer(name);
    func();
  } else {
    auto result = BenchmarkRunner::Benchmark(name, std::forward<Func>(func), iterations);
    BenchmarkRunner::PrintResult(result);
  }
}

}  // namespace cpp_features::timing