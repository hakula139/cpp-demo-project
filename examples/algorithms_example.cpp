/**
 * @file algorithms_example.cpp
 * @brief Example demonstrating the usage of the algorithms module
 */

#include <algorithm>
#include <cctype>
#include <print>
#include <string>

#include "algorithms/stl.hpp"
#include "containers/container.hpp"

using cpp_features::algorithms::CountIf;
using cpp_features::algorithms::FindMinMax;
using cpp_features::algorithms::SortContainer;
using cpp_features::algorithms::TransformToVector;
using cpp_features::containers::Container;

auto main() -> int {
  std::println("=== Algorithms Module Example ===");

  // Demonstrate SortContainer with integers
  Container<int> numbers{42, 17, 89, 3, 56, 23, 78, 12, 95, 34};
  std::println("Original numbers: {}", numbers);

  SortContainer(numbers);
  std::println("Sorted numbers: {}", numbers);

  // Demonstrate CountIf
  auto even_count = CountIf(numbers, [](int n) { return n % 2 == 0; });
  std::println("Count of even numbers: {}", even_count);

  auto large_count = CountIf(numbers, [](int n) { return n > 50; });
  std::println("Count of numbers > 50: {}", large_count);

  // Demonstrate TransformToVector (returns std::vector)
  auto squared = TransformToVector(numbers, [](int n) { return n * n; });
  std::println("Squared numbers: {}", Container(squared));

  // Demonstrate FindMinMax
  auto [min_val, max_val] = FindMinMax(numbers);
  std::println("Min: {}, Max: {}", min_val, max_val);

  // Demonstrate with strings
  Container<std::string> words{"cherry", "banana", "elderberry", "date", "apple"};
  std::println("Original words: {}", words);

  SortContainer(words);
  std::println("Sorted words: {}", words);

  auto long_words = CountIf(words, [](const std::string &word) { return word.length() > 5; });
  std::println("Count of words > 5 characters: {}", long_words);

  auto uppercased = TransformToVector(words, [](const std::string &word) {
    auto result = word;
    std::ranges::transform(result, result.begin(), ::toupper);
    return result;
  });
  std::println("Uppercased words: {}", Container(uppercased));

  auto [min_word, max_word] = FindMinMax(words);
  std::println("Min: {}, Max: {} (Lexicographically)", min_word, max_word);

  std::println("=== Algorithms Module Example Completed ===");
  return 0;
}
