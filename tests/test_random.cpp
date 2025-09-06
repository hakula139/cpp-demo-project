#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "random/random_gen.hpp"

namespace {

using namespace cpp_features::random;

TEST_CASE("RandomGenerator construction", "[random][generator]") {
  SECTION("Default constructor") {
    RandomGenerator generator;
    auto value = generator.GenerateInt(1, 10);

    REQUIRE(value >= 1);
    REQUIRE(value <= 10);
  }

  SECTION("Explicit seed constructor") {
    constexpr std::uint32_t SEED = 12345;

    RandomGenerator gen1{SEED};
    auto value1 = gen1.GenerateInt(1, 1000);

    RandomGenerator gen2{SEED};
    auto value2 = gen2.GenerateInt(1, 1000);

    REQUIRE(value1 == value2);
  }
}

TEST_CASE("RandomGenerator integral generation", "[random][generator][integral]") {
  RandomGenerator generator(42);  // Use fixed seed for reproducibility

  SECTION("Generate int values") {
    constexpr int MIN_VAL = 1;
    constexpr int MAX_VAL = 100;

    for (int i = 0; i < 100; ++i) {
      auto value = generator.GenerateInt(MIN_VAL, MAX_VAL);

      REQUIRE(value >= MIN_VAL);
      REQUIRE(value <= MAX_VAL);
    }
  }

  SECTION("Generate long values") {
    constexpr std::int64_t MIN_VAL = 1000L;
    constexpr std::int64_t MAX_VAL = 9999L;

    auto value = generator.GenerateInt(MIN_VAL, MAX_VAL);

    REQUIRE(value >= MIN_VAL);
    REQUIRE(value <= MAX_VAL);
  }

  SECTION("Generate single value range") {
    constexpr int SINGLE_VALUE = 42;

    auto value = generator.GenerateInt(SINGLE_VALUE, SINGLE_VALUE);

    REQUIRE(value == SINGLE_VALUE);
  }

  SECTION("Generate negative range") {
    constexpr int MIN_VAL = -100;
    constexpr int MAX_VAL = -10;

    auto value = generator.GenerateInt(MIN_VAL, MAX_VAL);

    REQUIRE(value >= MIN_VAL);
    REQUIRE(value <= MAX_VAL);
  }
}

TEST_CASE("RandomGenerator floating-point generation", "[random][generator][floating]") {
  RandomGenerator generator{123};  // Use fixed seed for reproducibility

  SECTION("Generate double values") {
    constexpr double MIN_VAL = 0.0;
    constexpr double MAX_VAL = 1.0;

    for (int i = 0; i < 100; ++i) {
      auto value = generator.GenerateReal(MIN_VAL, MAX_VAL);

      REQUIRE(value >= MIN_VAL);
      REQUIRE(value < MAX_VAL);  // max is exclusive for floating-point
    }
  }

  SECTION("Generate float values") {
    constexpr float MIN_VAL = -10.0F;
    constexpr float MAX_VAL = -5.0F;

    auto value = generator.GenerateReal(MIN_VAL, MAX_VAL);

    REQUIRE(value >= MIN_VAL);
    REQUIRE(value < MAX_VAL);
  }

  SECTION("Generate very small range") {
    constexpr double MIN_VAL = 0.0;
    constexpr double MAX_VAL = 0.001;

    auto value = generator.GenerateReal(MIN_VAL, MAX_VAL);

    REQUIRE(value >= MIN_VAL);
    REQUIRE(value < MAX_VAL);
  }
}

TEST_CASE("RandomGenerator vector generation", "[random][generator][vector]") {
  RandomGenerator generator{456};  // Use fixed seed for reproducibility

  SECTION("Generate integer vector") {
    constexpr int MIN_VAL = 1;
    constexpr int MAX_VAL = 49;
    constexpr std::size_t COUNT = 10;

    auto values = generator.GenerateIntVector(MIN_VAL, MAX_VAL, COUNT);

    REQUIRE(values.size() == COUNT);
    for (const auto &value : values) {
      REQUIRE(value >= MIN_VAL);
      REQUIRE(value <= MAX_VAL);
    }
  }

  SECTION("Generate floating-point vector") {
    constexpr double MIN_VAL = 0.0;
    constexpr double MAX_VAL = 1.0;
    constexpr std::size_t COUNT = 20;

    auto values = generator.GenerateRealVector(MIN_VAL, MAX_VAL, COUNT);

    REQUIRE(values.size() == COUNT);
    for (const auto &value : values) {
      REQUIRE(value >= MIN_VAL);
      REQUIRE(value < MAX_VAL);
    }
  }

  SECTION("Generate empty vector") {
    auto int_values = generator.GenerateIntVector(1, 10, 0);
    auto real_values = generator.GenerateRealVector(-10.0, -9.0, 0);

    REQUIRE(int_values.empty());
    REQUIRE(real_values.empty());
  }

  SECTION("Generate large vector") {
    constexpr std::size_t LARGE_COUNT = 1000;

    auto values = generator.GenerateIntVector(1, 100, LARGE_COUNT);

    REQUIRE(values.size() == LARGE_COUNT);
    // Check that we get some distribution (not all the same value)
    auto first_value = values[0];
    auto has_different =
        std::ranges::any_of(values, [first_value](int val) { return val != first_value; });
    REQUIRE(has_different);  // Should have some variety
  }
}

TEST_CASE("RandomGenerator boolean generation", "[random][generator][boolean]") {
  RandomGenerator generator{789};  // Use fixed seed for reproducibility

  SECTION("Fair coin flip (50%)") {
    constexpr std::size_t ITERATIONS = 1000;

    std::size_t true_count = 0;
    for (std::size_t i = 0; i < ITERATIONS; ++i) {
      if (generator.GenerateBool()) {
        ++true_count;
      }
    }

    // Should be roughly 50% (allow some variance)
    REQUIRE(true_count > static_cast<std::size_t>(ITERATIONS * 0.3));  // At least 30%
    REQUIRE(true_count < static_cast<std::size_t>(ITERATIONS * 0.7));  // At most 70%
  }

  SECTION("Biased coin (90% true)") {
    constexpr std::size_t ITERATIONS = 1000;
    constexpr double HIGH_PROBABILITY = 0.9;

    std::size_t true_count = 0;
    for (std::size_t i = 0; i < ITERATIONS; ++i) {
      if (generator.GenerateBool(HIGH_PROBABILITY)) {
        ++true_count;
      }
    }

    // Should be roughly 90% (allow some variance)
    REQUIRE(true_count > static_cast<std::size_t>(ITERATIONS * 0.7));  // At least 70%
  }

  SECTION("Low probability (10% true)") {
    constexpr std::size_t ITERATIONS = 1000;
    constexpr double LOW_PROBABILITY = 0.1;

    std::size_t true_count = 0;
    for (std::size_t i = 0; i < ITERATIONS; ++i) {
      if (generator.GenerateBool(LOW_PROBABILITY)) {
        ++true_count;
      }
    }

    // Should be roughly 10% (allow some variance)
    REQUIRE(true_count < static_cast<std::size_t>(ITERATIONS * 0.3));  // At most 30%
  }

  SECTION("Extreme probabilities") {
    constexpr std::size_t ITERATIONS = 1000;

    // Test 0% probability
    for (std::size_t i = 0; i < ITERATIONS; ++i) {
      REQUIRE_FALSE(generator.GenerateBool(0.0));
    }

    // Test 100% probability
    for (std::size_t i = 0; i < ITERATIONS; ++i) {
      REQUIRE(generator.GenerateBool(1.0));
    }
  }
}

TEST_CASE("RandomGenerator normal distribution", "[random][generator][normal]") {
  RandomGenerator generator{321};  // Use fixed seed for reproducibility

  SECTION("Standard normal distribution") {
    constexpr double MEAN = 0.0;
    constexpr double STDDEV = 1.0;
    constexpr std::size_t ITERATIONS = 1000;

    std::vector<double> values;
    values.reserve(ITERATIONS);

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
      values.push_back(generator.GenerateNormal(MEAN, STDDEV));
    }

    // Calculate sample mean and verify it's close to expected
    double sum = 0.0;
    for (const auto &value : values) {
      sum += value;
    }
    double sample_mean = sum / static_cast<double>(ITERATIONS);

    // Sample mean should be close to theoretical mean (within 0.2)
    REQUIRE(std::abs(sample_mean - MEAN) < 0.2);

    // Most values should be within 3 standard deviations
    auto within_3_sigma = std::ranges::count_if(
        values, [](double val) { return std::abs(val - MEAN) <= 3.0 * STDDEV; });
    // At least 99% should be within 3 sigma
    REQUIRE(within_3_sigma > static_cast<long>(ITERATIONS * 0.99));
  }

  SECTION("Custom normal distribution") {
    constexpr double MEAN = 100.0;
    constexpr double STDDEV = 15.0;

    auto value = generator.GenerateNormal(MEAN, STDDEV);

    // Value should be reasonable (within 5 standard deviations)
    REQUIRE(value > MEAN - (5.0 * STDDEV));
    REQUIRE(value < MEAN + (5.0 * STDDEV));
  }

  SECTION("Float normal distribution") {
    constexpr float MEAN = 5.0F;
    constexpr float STDDEV = 0.1F;

    auto value = generator.GenerateNormal(MEAN, STDDEV);

    // Value should be reasonable (within 5 standard deviations)
    REQUIRE(value > MEAN - (5.0F * STDDEV));
    REQUIRE(value < MEAN + (5.0F * STDDEV));
  }
}

TEST_CASE("RandomGenerator seeding", "[random][generator][seeding]") {
  SECTION("Manual seeding produces deterministic results") {
    constexpr std::uint32_t SEED = 12345;

    RandomGenerator gen1{SEED};
    RandomGenerator gen2{SEED};

    // Generate several values from both generators
    for (std::size_t i = 0; i < 10; ++i) {
      auto val1 = gen1.GenerateInt(1, 1000);
      auto val2 = gen2.GenerateInt(1, 1000);

      REQUIRE(val1 == val2);
    }
  }

  SECTION("Re-seeding resets the sequence") {
    constexpr std::uint32_t SEED = 54321;

    RandomGenerator generator;
    generator.Seed(SEED);
    auto first_sequence = generator.GenerateIntVector(1, 100, 100);

    generator.Seed(SEED);  // Reset with same seed
    auto second_sequence = generator.GenerateIntVector(1, 100, 100);

    REQUIRE(first_sequence == second_sequence);
  }

  SECTION("SeedWithTime provides non-deterministic behavior") {
    RandomGenerator generator;
    generator.SeedWithTime();
    auto first_sequence = generator.GenerateIntVector(1, 100, 100);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    generator.SeedWithTime();
    auto second_sequence = generator.GenerateIntVector(1, 100, 100);

    REQUIRE(first_sequence != second_sequence);
  }
}

TEST_CASE("ShuffleContainer function", "[random][shuffle]") {
  SECTION("Shuffle vector") {
    std::vector<int> original;
    for (int i = 1; i <= 20; ++i) {
      original.push_back(i);
    }

    std::vector<int> shuffled = original;
    ShuffleContainer(shuffled);

    // Size should remain the same
    REQUIRE(shuffled.size() == original.size());

    // Should be different (very high probability)
    REQUIRE(shuffled != original);

    // Should contain all the same elements
    std::ranges::sort(shuffled);
    REQUIRE(shuffled == original);
  }

  SECTION("Shuffle array") {
    std::array<char, 26> original{
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    };

    std::array<char, 26> shuffled = original;
    ShuffleContainer(shuffled);

    // Size should remain the same
    REQUIRE(shuffled.size() == original.size());

    // Should be different (very high probability)
    REQUIRE(shuffled != original);

    // Should contain all the same elements
    std::ranges::sort(shuffled);
    REQUIRE(shuffled == original);
  }

  SECTION("Shuffle single element") {
    std::vector<int> single{42};

    ShuffleContainer(single);

    REQUIRE(single.size() == 1);
    REQUIRE(single[0] == 42);
  }

  SECTION("Shuffle empty container") {
    std::vector<int> empty;

    ShuffleContainer(empty);

    REQUIRE(empty.empty());
  }
}

TEST_CASE("SampleFromRange function", "[random][sample]") {
  SECTION("Sample from vector") {
    std::vector<int> population{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    constexpr std::size_t SAMPLE_SIZE = 3;

    auto sample = SampleFromRange(population, SAMPLE_SIZE);

    REQUIRE(sample.size() == SAMPLE_SIZE);

    // All sampled elements should be from the original population
    for (const auto &element : sample) {
      REQUIRE(std::ranges::find(population, element) != population.end());
    }
  }

  SECTION("Sample from string") {
    std::string text = "abcdefghijklmnop";
    constexpr std::size_t SAMPLE_SIZE = 5;

    auto sample = SampleFromRange(text, SAMPLE_SIZE);

    REQUIRE(sample.size() == SAMPLE_SIZE);

    // All sampled characters should be from the original string
    for (auto c : sample) {
      REQUIRE(text.find(c) != std::string::npos);
    }
  }

  SECTION("Sample more than available") {
    std::vector<int> small_population{1, 2, 3};
    constexpr std::size_t LARGE_SAMPLE_SIZE = 10;

    auto sample = SampleFromRange(small_population, LARGE_SAMPLE_SIZE);

    // Should return all available elements
    std::ranges::sort(sample);
    REQUIRE(sample == small_population);
  }

  SECTION("Sample zero elements") {
    std::vector<int> population{1, 2, 3, 4, 5};

    auto sample = SampleFromRange(population, 0);

    REQUIRE(sample.empty());
  }

  SECTION("Sample from empty range") {
    std::vector<int> empty_population;

    auto sample = SampleFromRange(empty_population, 5);

    REQUIRE(sample.empty());
  }
}

}  // namespace
