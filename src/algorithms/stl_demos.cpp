#include <vector>
#include <algorithm>
#include <ranges>
#include <print>
#include <numeric>
#include <chrono>
#include <functional>

#include "algorithms/stl_demos.hpp"

namespace cpp_features::algorithms {

void DemonstrateStlAlgorithms() {
  std::print("\n=== STL Algorithms Demonstration ===\n");

  std::vector<int> data = {42, 17, 89, 3, 56, 23, 78, 12, 95, 34};

  std::print("Original data: ");
  for (const auto& n : data) {
    std::print("{} ", n);
  }
  std::print("\n");

  auto sorted_data = data;
  SortContainer(sorted_data);
  std::print("Sorted data: ");
  for (const auto& n : sorted_data) {
    std::print("{} ", n);
  }
  std::print("\n");

  auto even_count = CountIf(data, [](int n) { return n % 2 == 0; });
  std::print("Even numbers count: {}\n", even_count);

  auto squared = TransformToVector(data, [](int n) { return n * n; });
  std::print("Squared values: ");
  for (const auto& n : squared) {
    std::print("{} ", n);
  }
  std::print("\n");

  auto [min_val, max_val] = FindMinMax(data);
  std::print("Min: {}, Max: {}\n", min_val, max_val);
}

void DemonstrateRangesAndViews() {
  std::print("\n=== Ranges and Views Demonstration ===\n");

  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  auto even_squares = numbers
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; });

  std::print("Even squares: ");
  for (const auto& n : even_squares) {
    std::print("{} ", n);
  }
  std::print("\n");

  auto first_three_odds = numbers
    | std::views::filter([](int n) { return n % 2 == 1; })
    | std::views::take(3);

  std::print("First three odds: ");
  for (const auto& n : first_three_odds) {
    std::print("{} ", n);
  }
  std::print("\n");

  auto reversed_view = numbers | std::views::reverse;
  std::print("Reversed: ");
  for (const auto& n : reversed_view) {
    std::print("{} ", n);
  }
  std::print("\n");
}

void DemonstrateParallelAlgorithms() {
  std::print("\n=== Parallel Algorithms Demonstration ===\n");

  std::vector<int> large_data(1000);
  std::iota(large_data.begin(), large_data.end(), 1);

  auto start = std::chrono::high_resolution_clock::now();
  std::sort(large_data.begin(), large_data.end(), std::greater<int>());
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::print("Sorted {} elements in {}μs\n", large_data.size(), duration.count());

  auto sum = std::accumulate(large_data.begin(), large_data.end(), 0LL);
  std::print("Sum of all elements: {}\n", sum);
}

void DemonstrateCustomAlgorithms() {
  std::print("\n=== Custom Algorithms Demonstration ===\n");

  std::vector<std::string> words = {"apple", "banana", "cherry", "date", "elderberry"};

  auto long_words = CountIf(words, [](const std::string& word) {
    return word.length() > 5;
  });
  std::print("Words longer than 5 characters: {}\n", long_words);

  auto uppercased = TransformToVector(words, [](const std::string& word) {
    std::string result = word;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
  });

  std::print("Uppercased words: ");
  for (const auto& word : uppercased) {
    std::print("{} ", word);
  }
  std::print("\n");

  std::vector<double> scores = {85.5, 92.0, 78.5, 96.0, 88.5};
  auto [min_score, max_score] = FindMinMax(scores);
  std::print("Score range: {:.1f} - {:.1f}\n", min_score, max_score);
}

}  // namespace cpp_features::algorithms
