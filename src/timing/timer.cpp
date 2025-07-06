/**
 * @file timer.cpp
 * @brief Implementation of timing utilities
 */

#include "timing/timer.hpp"

#include <chrono>
#include <cstdint>
#include <format>
#include <string>

namespace cpp_features::timing {

auto ToHumanReadable(std::int64_t ns) noexcept -> std::string {
  if (ns < NANOSECONDS_PER_MICROSECOND) {
    return std::format("{}ns", ns);
  }
  if (ns < NANOSECONDS_PER_MILLISECOND) {
    return std::format("{:.2f}μs", static_cast<double>(ns) / NANOSECONDS_PER_MICROSECOND);
  }
  if (ns < NANOSECONDS_PER_SECOND) {
    return std::format("{:.2f}ms", static_cast<double>(ns) / NANOSECONDS_PER_MILLISECOND);
  }
  return std::format("{:.2f}s", static_cast<double>(ns) / NANOSECONDS_PER_SECOND);
}

auto Timer::GetElapsedString() const noexcept -> std::string {
  return ToHumanReadable(GetElapsed<std::chrono::nanoseconds>());
}

}  // namespace cpp_features::timing
