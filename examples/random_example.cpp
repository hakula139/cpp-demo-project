/**
 * @file random_example.cpp
 * @brief Example demonstrating the usage of the random module
 */

#include <cstddef>
#include <numeric>
#include <print>
#include <string_view>

#include "containers/container.hpp"
#include "random/random_gen.hpp"

using cpp_features::containers::Container;
using cpp_features::random::RandomGenerator;
using cpp_features::random::SampleFromRange;
using cpp_features::random::ShuffleContainer;

namespace {

/**
 * @brief Demonstrate basic random number generation
 */
void DemonstrateBasicGeneration() {
  std::println("\n1. Basic Random Generation:");
  RandomGenerator generator{42};  // Fixed seed for reproducible output

  // Integer generation
  auto dice_roll = generator.GenerateInt(1, 6);
  auto lottery_number = generator.GenerateInt(1, 49);
  std::println("   Dice roll: {}, Lottery number: {}", dice_roll, lottery_number);

  // Float generation
  auto probability = generator.GenerateReal(0.0, 1.0);
  auto temperature = generator.GenerateReal(-10.0F, 40.0F);
  std::println("   Probability: {:.3f}, Temperature: {:.1f}°C", probability, temperature);

  // Boolean generation
  std::string_view coin_flip{generator.GenerateBool() ? "Heads" : "Tails"};
  std::string_view rare_event{generator.GenerateBool(0.1) ? "Occurred" : "Didn't occur"};
  std::println("   Coin flip: {}, Rare event: {}", coin_flip, rare_event);
}

/**
 * @brief Demonstrate various probability distributions
 */
void DemonstrateProbabilityDistributions() {
  std::println("\n2. Probability Distributions:");
  RandomGenerator generator{42};

  // Uniform distributions
  Container uniform_ints(generator.GenerateIntVector(10, 20, 5));
  std::println("   Uniform integers [10, 20]: {}", uniform_ints);

  Container uniform_reals(generator.GenerateRealVector(0.5F, 1.5F, 3));
  std::println("   Uniform reals [0.5, 1.5): {}", uniform_reals);

  // Normal distribution
  Container<int> iq_scores;
  for (std::size_t i = 0; i < 5; ++i) {
    iq_scores.Add(static_cast<int>(generator.GenerateNormal(100.0, 15.0)));
  }
  std::println("   IQ scores (μ = 100, σ = 15): {}", iq_scores);

  // Bernoulli distribution
  Container<std::string_view> coin_flips;
  for (std::size_t i = 0; i < 10; ++i) {
    coin_flips.Add(generator.GenerateBool(0.7) ? "H" : "T");
  }
  std::println("   Biased coin flips (p = 0.7): {}", coin_flips);
}

/**
 * @brief Demonstrate seeding and reproducibility
 */
void DemonstrateSeedingAndReproducibility() {
  std::println("\n3. Seeding and Reproducibility:");
  RandomGenerator gen1{123};
  RandomGenerator gen2{123};
  auto val1 = gen1.GenerateInt(1, 1000);
  auto val2 = gen2.GenerateInt(1, 1000);
  std::println("   Same seed (123): {} == {}", val1, val2);

  RandomGenerator gen3;  // Random seed
  gen3.SeedWithTime();
  auto time_seeded = gen3.GenerateInt(1, 1000);
  std::println("   Time-seeded value: {}", time_seeded);
}

/**
 * @brief Demonstrate range operations (shuffling and sampling)
 */
void DemonstrateRangeOperations() {
  std::println("\n4. Range Operations:");

  // Container shuffling
  Container deck{1, 2, 3, 4, 5, 6, 7, 8};
  std::println("   Original deck: {}", deck);

  ShuffleContainer(deck);
  std::println("   Shuffled deck: {}", deck);

  // Random sampling
  Container population{10, 20, 30, 40, 50, 60, 70, 80, 90};
  Container sample(SampleFromRange(population, 4));
  std::println("   Sample from population: {}", sample);

  // String sampling
  std::string_view vowels{"AEIOU"};
  Container vowel_sample(SampleFromRange(vowels, 3));
  std::println("   Random vowels: {}", vowel_sample);
}

/**
 * @brief Demonstrate practical applications of random generation
 */
void DemonstratePracticalApplications() {
  std::println("\n5. Practical Applications:");
  RandomGenerator dice_generator;

  // Simulate rolling multiple dice
  Container dice_rolls(dice_generator.GenerateIntVector(1, 6, 3));
  auto total = std::reduce(dice_rolls.begin(), dice_rolls.end(), 0);
  std::println("   Rolling 3 dice: {}, Total: {}", dice_rolls, total);

  // Monte Carlo estimation (simple π approximation)
  auto inside_circle = 0;
  constexpr auto SAMPLES = 10000;
  for (std::size_t i = 0; i < SAMPLES; ++i) {
    auto x = dice_generator.GenerateReal(-1.0, 1.0);
    auto y = dice_generator.GenerateReal(-1.0, 1.0);
    if (x * x + y * y <= 1.0) {
      ++inside_circle;
    }
  }
  auto pi_estimate = 4.0 * inside_circle / SAMPLES;
  std::println("   Monte Carlo π estimation ({} samples): {:.4f}", SAMPLES, pi_estimate);
}

}  // namespace

auto main() -> int {
  std::println("=== Random Module Example ===");

  DemonstrateBasicGeneration();
  DemonstrateProbabilityDistributions();
  DemonstrateSeedingAndReproducibility();
  DemonstrateRangeOperations();
  DemonstratePracticalApplications();

  std::println("\n=== Random Module Example Completed ===");
  return 0;
}
