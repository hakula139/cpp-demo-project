/**
 * @file random_gen.hpp
 * @brief Modern C++ random number generation utilities with concepts and ranges support
 *
 * This module provides random number generation utilities that demonstrate the use of concepts,
 * ranges, and modern C++ random number generation facilities.
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <random>
#include <ranges>
#include <vector>

namespace cpp_features::random {

/**
 * @brief Modern random number generator with type-safe operations
 *
 * The RandomGenerator class provides a comprehensive interface for generating random numbers with
 * various distributions while ensuring type safety through concepts. It wraps the Mersenne Twister
 * generator (std::mt19937) and provides convenient methods for common random generation tasks.
 *
 * Features:
 *
 * - Type-safe random number generation with concept constraints
 * - Support for uniform distributions (integral and floating-point)
 * - Normal distribution support
 * - Boolean generation with configurable probability
 * - Vector generation methods for bulk random value creation
 * - Flexible seeding options (manual, time-based, or entropy-based)
 *
 * The class is designed to be used as a single instance for related random operations to maintain
 * consistent state and avoid repeated seeding overhead.
 *
 * @code
 * RandomGenerator generator;
 *
 * // Generate single values
 * auto random_int = generator.GenerateInt(1, 100);
 * auto random_double = generator.GenerateReal(0.0, 1.0);
 * auto coin_flip = generator.GenerateBool(0.5);
 *
 * // Generate vectors of random values
 * auto random_ints = generator.GenerateIntVector(1, 100, 50);
 * auto random_doubles = generator.GenerateRealVector(0.0, 1.0, 50);
 *
 * // Use different distributions
 * auto normal_value = generator.GenerateNormal(0.0, 1.0);
 * @endcode
 */
class RandomGenerator {
 public:
  /**
   * @brief Default constructor with entropy-based seeding
   *
   * Creates a RandomGenerator instance and seeds it using std::random_device for non-deterministic
   * initialization. This provides good randomness for most use cases where reproducibility is not
   * required.
   */
  RandomGenerator();

  /**
   * @brief Constructor with explicit seed value
   *
   * @param seed The seed value to initialize the random number generator
   *
   * Creates a RandomGenerator instance with a specific seed value. This is useful for reproducible
   * random sequences in testing, simulations, or when deterministic behavior is required.
   *
   * @code
   * RandomGenerator generator{12345};  // Deterministic seed
   * @endcode
   */
  explicit RandomGenerator(std::uint32_t seed);

  /**
   * @brief Generate a random integral value within a range
   *
   * @tparam T Integral type that satisfies std::integral concept
   * @param min Minimum value (inclusive)
   * @param max Maximum value (inclusive)
   * @return Random value of type T in the range [min, max]
   *
   * Generates a uniformly distributed random integral value within the specified range.
   * Both bounds are inclusive. The type T must be an integral type (int, long, etc.).
   *
   * @code
   * auto dice_roll = generator.GenerateInt(1, 6);           // int
   * auto big_number = generator.GenerateInt(1000L, 9999L);  // long
   * @endcode
   */
  template <std::integral T>
  [[nodiscard]] auto GenerateInt(T min, T max) -> T {
    std::uniform_int_distribution<T> dist{min, max};
    return dist(generator_);
  }

  /**
   * @brief Generate a random floating-point value within a range
   *
   * @tparam T Floating-point type that satisfies std::floating_point concept
   * @param min Minimum value (inclusive)
   * @param max Maximum value (exclusive)
   * @return Random value of type T in the range [min, max)
   *
   * Generates a uniformly distributed random floating-point value within the specified range.
   * The minimum bound is inclusive, the maximum bound is exclusive. The type T must be a
   * floating-point type (float, double, long double).
   *
   * @code
   * auto probability = generator.GenerateReal(0.0, 1.0);       // double
   * auto temperature = generator.GenerateReal(-10.0F, 40.0F);  // float
   * @endcode
   */
  template <std::floating_point T>
  [[nodiscard]] auto GenerateReal(T min, T max) -> T {
    std::uniform_real_distribution<T> dist{min, max};
    return dist(generator_);
  }

  /**
   * @brief Generate a vector of random integral values
   *
   * @tparam T Integral type that satisfies std::integral concept
   * @param min Minimum value for each element (inclusive)
   * @param max Maximum value for each element (inclusive)
   * @param count Number of random values to generate
   * @return Vector containing count random values of type T
   *
   * Efficiently generates a vector of uniformly distributed random integral values.
   * Each value is independently generated within the specified range.
   *
   * @code
   * auto lottery_numbers = generator.GenerateIntVector(1, 49, 6);
   * auto test_data = generator.GenerateIntVector(-100L, 100L, 1000);
   * @endcode
   */
  template <std::integral T>
  [[nodiscard]] auto GenerateIntVector(T min, T max, std::size_t count) -> std::vector<T> {
    std::vector<T> result;
    result.reserve(count);
    std::uniform_int_distribution<T> dist{min, max};

    for (std::size_t i = 0; i < count; ++i) {
      result.push_back(dist(generator_));
    }
    return result;
  }

  /**
   * @brief Generate a vector of random floating-point values
   *
   * @tparam T Floating-point type that satisfies std::floating_point concept
   * @param min Minimum value for each element (inclusive)
   * @param max Maximum value for each element (exclusive)
   * @param count Number of random values to generate
   * @return Vector containing count random values of type T
   *
   * Efficiently generates a vector of uniformly distributed random floating-point values.
   * Each value is independently generated within the specified range.
   *
   * @code
   * auto samples = generator.GenerateRealVector(0.0, 1.0, 10000);
   * auto coordinates = generator.GenerateRealVector(-1.0F, 1.0F, 100);
   * @endcode
   */
  template <std::floating_point T>
  [[nodiscard]] auto GenerateRealVector(T min, T max, std::size_t count) -> std::vector<T> {
    std::vector<T> result;
    result.reserve(count);
    std::uniform_real_distribution<T> dist{min, max};

    for (std::size_t i = 0; i < count; ++i) {
      result.push_back(dist(generator_));
    }
    return result;
  }

  /**
   * @brief Generate a random boolean value with specified probability
   *
   * @param probability Probability of returning true (default: 0.5)
   * @return Random boolean value
   *
   * Generates a random boolean value using a Bernoulli distribution with the specified probability.
   * A probability of 0.5 creates a fair coin flip, while other values bias the outcome accordingly.
   *
   * @code
   * auto coin_flip = generator.GenerateBool();       // 50% chance of true
   * auto biased_coin = generator.GenerateBool(0.7);  // 70% chance of true
   * auto rare_event = generator.GenerateBool(0.01);  // 1% chance of true
   * @endcode
   */
  [[nodiscard]] auto GenerateBool(double probability = 0.5) -> bool {
    std::bernoulli_distribution dist{probability};
    return dist(generator_);
  }

  /**
   * @brief Generate a random value from a normal (Gaussian) distribution
   *
   * @tparam T Floating-point type that satisfies std::floating_point concept
   * @param mean Mean (center) of the distribution
   * @param stddev Standard deviation of the distribution
   * @return Random value from the normal distribution
   *
   * Generates a random value from a normal (Gaussian) distribution with the specified mean and
   * standard deviation. This is useful for generating naturally distributed data, noise, or
   * measurements with known statistics.
   *
   * @code
   * auto standard_normal = generator.GenerateNormal(0.0, 1.0);  // Standard normal
   * auto iq_score = generator.GenerateNormal(100.0, 15.0);      // IQ distribution
   * auto measurement = generator.GenerateNormal(5.0F, 0.1F);    // Measurement with error
   * @endcode
   */
  template <std::floating_point T>
  [[nodiscard]] auto GenerateNormal(T mean, T stddev) -> T {
    std::normal_distribution<T> dist{mean, stddev};
    return dist(generator_);
  }

  /**
   * @brief Manually seed the random number generator
   *
   * @param seed The seed value to use for initialization
   *
   * Sets a specific seed for the random number generator. This allows for reproducible random
   * sequences, which is useful for testing, debugging, or when deterministic behavior is required.
   *
   * @code
   * generator.Seed(42);  // Use deterministic seed
   * @endcode
   */
  void Seed(std::uint32_t seed) { generator_.seed(seed); }

  /**
   * @brief Seed the generator using current time
   *
   * Re-seeds the random number generator using the current high-resolution time.
   * This provides non-deterministic behavior and is useful when you want to reset the generator
   * with a new unpredictable seed.
   *
   * @code
   * generator.SeedWithTime();  // Non-deterministic re-seeding
   * @endcode
   */
  void SeedWithTime() {
    auto now = std::chrono::high_resolution_clock::now();
    auto seed = static_cast<std::uint32_t>(now.time_since_epoch().count());
    generator_.seed(seed);
  }

 private:
  std::mt19937 generator_;  ///< Mersenne Twister random number generator
};

/**
 * @brief Randomly shuffle elements in a container
 *
 * @tparam Range Type that satisfies std::ranges::random_access_range concept
 * @param range The range to shuffle (modified in-place)
 *
 * Randomly reorders the elements in the provided range using the Fisher-Yates shuffle algorithm.
 * The range must support random access for efficient shuffling. Uses thread-local random generation
 * for thread safety.
 *
 * @code
 * std::vector<int> deck{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 * ShuffleContainer(deck);
 * // deck is now randomly shuffled
 *
 * std::array<char, 4> letters{'A', 'B', 'C', 'D'};
 * ShuffleContainer(letters);
 * @endcode
 */
template <std::ranges::random_access_range Range>
void ShuffleContainer(Range &range) {
  thread_local std::random_device rd;
  thread_local std::mt19937 gen{rd()};
  std::shuffle(std::ranges::begin(range), std::ranges::end(range), gen);
}

/**
 * @brief Randomly sample elements from a range
 *
 * @tparam Range Type that satisfies std::ranges::input_range concept
 * @param range The source range to sample from
 * @param count Number of elements to sample
 * @return Vector containing randomly selected elements from the range
 *
 * Selects a random subset of elements from the input range without replacement.
 * If count exceeds the range size, all elements are returned. The relative order of sampled
 * elements is preserved. Uses thread-local random generation for thread safety.
 *
 * @code
 * std::vector<int> population{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 * auto sample = SampleFromRange(population, 3);
 * // sample contains 3 randomly selected elements
 *
 * std::string text = "abcdefghijklmnop";
 * auto char_sample = SampleFromRange(text, 5);
 * @endcode
 */
template <std::ranges::input_range Range>
[[nodiscard]] auto SampleFromRange(Range &&range, std::size_t count)
    -> std::vector<std::ranges::range_value_t<Range>> {
  std::vector<std::ranges::range_value_t<Range>> result;
  thread_local std::random_device rd;
  thread_local std::mt19937 gen{rd()};
  std::sample(std::ranges::begin(range), std::ranges::end(range), std::back_inserter(result), count,
              gen);
  return result;
}

}  // namespace cpp_features::random
