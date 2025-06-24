#pragma once

#include <random>
#include <vector>
#include <ranges>
#include <concepts>
#include <chrono>
#include <algorithm>

#include "concepts/arithmetic_concepts.hpp"

namespace cpp_features::random {

template<typename T>
concept IntegralType = std::integral<T>;

template<typename T>
concept FloatingPointType = std::floating_point<T>;

class RandomGenerator {
 public:
  RandomGenerator();
  explicit RandomGenerator(std::uint32_t seed);

  template<IntegralType T>
  [[nodiscard]] auto GenerateInt(T min, T max) -> T {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(generator_);
  }

  template<FloatingPointType T>
  [[nodiscard]] auto GenerateReal(T min, T max) -> T {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(generator_);
  }

  template<IntegralType T>
  [[nodiscard]] auto GenerateIntVector(T min, T max, std::size_t count) -> std::vector<T> {
    std::vector<T> result;
    result.reserve(count);
    std::uniform_int_distribution<T> dist(min, max);

    for (std::size_t i = 0; i < count; ++i) {
      result.push_back(dist(generator_));
    }
    return result;
  }

  template<FloatingPointType T>
  [[nodiscard]] auto GenerateRealVector(T min, T max, std::size_t count) -> std::vector<T> {
    std::vector<T> result;
    result.reserve(count);
    std::uniform_real_distribution<T> dist(min, max);

    for (std::size_t i = 0; i < count; ++i) {
      result.push_back(dist(generator_));
    }
    return result;
  }

  [[nodiscard]] auto GenerateBool(double probability = 0.5) -> bool {
    std::bernoulli_distribution dist(probability);
    return dist(generator_);
  }

  template<FloatingPointType T>
  [[nodiscard]] auto GenerateNormal(T mean, T stddev) -> T {
    std::normal_distribution<T> dist(mean, stddev);
    return dist(generator_);
  }

  void Seed(std::uint32_t seed) {
    generator_.seed(seed);
  }

  void SeedWithTime() {
    auto now = std::chrono::high_resolution_clock::now();
    auto seed = static_cast<std::uint32_t>(now.time_since_epoch().count());
    generator_.seed(seed);
  }

 private:
  std::mt19937 generator_;
};

void DemonstrateBasicRandomGeneration();

void DemonstrateDistributions();

void DemonstrateRandomRanges();

template<std::ranges::random_access_range Range>
void ShuffleContainer(Range&& range) {
  thread_local std::random_device rd;
  thread_local std::mt19937 gen(rd());
  std::shuffle(std::ranges::begin(range), std::ranges::end(range), gen);
}

template<std::ranges::input_range Range>
[[nodiscard]] auto SampleFromRange(Range&& range, std::size_t count)
  -> std::vector<std::ranges::range_value_t<Range>> {
  std::vector<std::ranges::range_value_t<Range>> result;
  thread_local std::random_device rd;
  thread_local std::mt19937 gen(rd());
  std::sample(std::ranges::begin(range), std::ranges::end(range),
             std::back_inserter(result), count, gen);
  return result;
}

}  // namespace cpp_features::random