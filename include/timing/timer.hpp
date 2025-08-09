/**
 * @file timer.hpp
 * @brief Modern C++ timing utilities with high-resolution timing capabilities
 *
 * This module provides timing utilities for measuring code execution time. It includes
 * high-resolution timer classes and scoped timing utilities that demonstrate modern C++ chrono
 * features and RAII patterns.
 */

#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <print>
#include <string>
#include <string_view>

#include "concepts/callable_concepts.hpp"

namespace cpp_features::timing {

/// High-resolution clock type for precise timing measurements
using Clock = std::chrono::high_resolution_clock;

/// Time point type for representing specific moments in time
using TimePoint = Clock::time_point;

/// Duration type for representing time intervals
using Duration = Clock::duration;

/**
 * @brief Alias template for time intervals with specific units
 *
 * @tparam Rep The representation type (e.g., int, double)
 * @tparam Period The period type (e.g., std::milli, std::nano)
 */
template <typename Rep, typename Period>
using TimeInterval = std::chrono::duration<Rep, Period>;

/// Constants for converting between time units
constexpr auto NANOSECONDS_PER_MICROSECOND = 1'000;
constexpr auto NANOSECONDS_PER_MILLISECOND = 1'000'000;
constexpr auto NANOSECONDS_PER_SECOND = 1'000'000'000;

/**
 * @brief Convert a duration in nanoseconds to a human-readable string
 *
 * @param ns The duration in nanoseconds
 * @return A formatted string representing the duration with appropriate units
 *
 * Automatically selects the most appropriate unit (ns, μs, ms, s) based on the magnitude of the
 * duration for optimal readability.
 *
 * @code
 * std::println("Operation took {}", ToHumanReadable(123'456'789));
 * // Output: "Operation took 123.46ms"
 * @endcode
 */
[[nodiscard]] auto ToHumanReadable(std::int64_t ns) noexcept -> std::string;

/**
 * @brief High-resolution timer class for measuring elapsed time
 *
 * The Timer class provides high-resolution timing capabilities using std::chrono.
 * It supports starting, stopping, and resetting measurements, with flexible duration type
 * conversion and string formatting.
 *
 * Features:
 *
 * - High-resolution timing using std::chrono::high_resolution_clock
 * - Flexible duration type conversion (nanoseconds, microseconds, milliseconds, seconds)
 * - Automatic string formatting with appropriate units
 * - Reset functionality for multiple measurements
 * - Continuous timing support (measures until explicitly stopped)
 *
 * @code
 * Timer timer;
 * // Do some work...
 * timer.Stop();
 * std::println("Elapsed time: {}", timer.GetElapsedString());
 * std::println("Elapsed ms: {}", timer.GetElapsed<std::chrono::milliseconds>());
 * @endcode
 */
class Timer {
 public:
  /**
   * @brief Construct a timer and start timing immediately
   *
   * Creates a new timer instance and captures the current time as the start point.
   * The timer begins measuring elapsed time immediately upon construction.
   */
  Timer() : start_time_{Clock::now()} {}

  /**
   * @brief Start or restart the timer
   *
   * Captures the current time as the start point for timing measurements.
   * If the timer was already running, this restarts the measurement.
   */
  void Start() noexcept { start_time_ = Clock::now(); }

  /**
   * @brief Stop the timer and record the end time
   *
   * Captures the current time as the end point for timing measurements.
   * After calling Stop(), GetElapsed() methods will return the fixed duration between Start() and
   * Stop() calls.
   */
  void Stop() noexcept { end_time_ = Clock::now(); }

  /**
   * @brief Get the elapsed time in the specified duration type
   *
   * @tparam DurationType The duration type for the result (default: std::chrono::milliseconds)
   * @return The elapsed time count in the specified duration type
   *
   * Returns the elapsed time between the start time and either the stop time (if Stop() was called)
   * or the current time (if still running).
   *
   * @code
   * auto elapsed_ns = timer.GetElapsed<std::chrono::nanoseconds>();
   * auto elapsed_us = timer.GetElapsed<std::chrono::microseconds>();
   * auto elapsed_ms = timer.GetElapsed();  // defaults to milliseconds
   * @endcode
   */
  template <typename DurationType = std::chrono::milliseconds>
  [[nodiscard]] auto GetElapsed() const noexcept -> typename DurationType::rep {
    return GetElapsedDuration<DurationType>().count();
  }

  /**
   * @brief Get the elapsed time as a duration object
   *
   * @tparam DurationType The duration type for the result (default: std::chrono::milliseconds)
   * @return The elapsed time as a duration object
   *
   * Returns the elapsed time as a duration object rather than a raw count.
   * This is useful when you need to perform duration arithmetic or comparisons.
   *
   * @code
   * auto duration = timer.GetElapsedDuration();
   * if (duration > std::chrono::milliseconds(100)) {
   *   std::println("Operation took longer than 100ms");
   * }
   * @endcode
   */
  template <typename DurationType = std::chrono::milliseconds>
  [[nodiscard]] auto GetElapsedDuration() const noexcept -> DurationType {
    auto end = (end_time_ == TimePoint{}) ? Clock::now() : end_time_;
    return std::chrono::duration_cast<DurationType>(end - start_time_);
  }

  /**
   * @brief Get the elapsed time as a formatted string with appropriate units
   *
   * @return A formatted string representing the elapsed time with units
   *
   * Automatically selects the most appropriate unit (ns, μs, ms, s) based on the magnitude of the
   * elapsed time for optimal readability.
   *
   * @code
   * std::println("Operation completed in {}", timer.GetElapsedString());
   * // Output examples: "1.23ms", "456.78μs", "1.23s", "789ns"
   * @endcode
   */
  [[nodiscard]] auto GetElapsedString() const noexcept -> std::string;

  /**
   * @brief Reset the timer to start a new measurement
   *
   * Resets the timer by capturing the current time as the new start point and clearing any
   * previously recorded end time. This allows reusing the same timer instance for multiple
   * measurements.
   */
  void Reset() noexcept {
    start_time_ = Clock::now();
    end_time_ = TimePoint{};
  }

 private:
  TimePoint start_time_;  ///< The time point when timing started
  TimePoint end_time_;    ///< The time point when timing stopped
};

/**
 * @brief RAII-based scoped timer that automatically measures execution time
 *
 * The ScopedTimer class provides automatic timing measurements using RAII principles.
 * It starts timing upon construction and automatically prints the elapsed time when the scope ends
 * (destructor is called). Optionally supports a callback function to receive the timing results.
 *
 * This is particularly useful for measuring the execution time of code blocks without manual timer
 * management.
 *
 * @code
 * {
 *   ScopedTimer timer{"Database query"};
 *   // Perform database operations...
 * } // Automatically prints: "Database query: 45.67ms"
 *
 * {
 *   ScopedTimer timer{"Complex calculation", [](std::int64_t ns) {
 *     if (ns > 1'000'000) {  // 1ms
 *       std::println("Warning: Calculation took {}ms", static_cast<double>(ns) / 1'000'000);
 *     }
 *   });
 *   // Perform calculations...
 * } // Prints timing and calls callback
 * @endcode
 */
class ScopedTimer {
 public:
  /**
   * @brief Construct a scoped timer with a descriptive name
   *
   * @param name Descriptive name for the timed operation
   *
   * Creates a scoped timer that will automatically print the elapsed time when the scope ends,
   * using the provided name as a label.
   */
  explicit ScopedTimer(std::string_view name) : name_{name} {}

  /**
   * @brief Construct a scoped timer with a name and callback function
   *
   * @tparam Func Callable type for the callback function that accepts std::int64_t and returns void
   * @param name Descriptive name for the timed operation
   * @param callback Function to call with the elapsed time in nanoseconds
   *
   * Creates a scoped timer that will automatically print the elapsed time and call the provided
   * callback function when the scope ends.
   */
  template <concepts::TimerCallback Func>
  explicit ScopedTimer(std::string_view name, Func callback) : name_{name}, callback_{callback} {}

  /**
   * @brief Destructor that automatically prints timing results
   *
   * Automatically stops the timer, prints the elapsed time with the provided name, and calls the
   * callback function (if provided) with the elapsed time in nanoseconds.
   */
  ~ScopedTimer() noexcept {
    timer_.Stop();
    std::println("{}: {}", name_, timer_.GetElapsedString());

    if (callback_) {
      callback_(timer_.GetElapsed<std::chrono::nanoseconds>());
    }
  }

 private:
  std::string_view name_;                       ///< Descriptive name for the timed operation
  Timer timer_;                                 ///< Internal timer for measurements
  std::function<void(std::int64_t)> callback_;  ///< Optional callback function for results
};

/**
 * @brief Utility function to time a single function execution
 *
 * @tparam Func Callable type (function, lambda, functor) that can be called without arguments
 * @param func Function to time
 * @return Elapsed time in nanoseconds
 *
 * Executes the provided function once and returns the elapsed time.
 * This is a convenience function for simple timing measurements.
 *
 * @code
 * auto elapsed = TimeFunction([]() {
 *   std::this_thread::sleep_for(std::chrono::milliseconds(100));
 * });
 * std::println("Function took {}ns", elapsed);
 * @endcode
 */
template <concepts::NullaryCallable Func>
[[nodiscard]] auto TimeFunction(Func &&func) -> std::int64_t {
  Timer timer;
  static_cast<void>(func());
  timer.Stop();
  return timer.GetElapsed<std::chrono::nanoseconds>();
}

}  // namespace cpp_features::timing
