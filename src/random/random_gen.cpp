/**
 * @file random_gen.cpp
 * @brief Implementation of random number generator utility class
 */

#include "random/random_gen.hpp"

#include <cstdint>
#include <random>

namespace cpp_features::random {

RandomGenerator::RandomGenerator() : generator_{std::random_device{}()} {}

RandomGenerator::RandomGenerator(std::uint32_t seed) : generator_{seed} {}

}  // namespace cpp_features::random
