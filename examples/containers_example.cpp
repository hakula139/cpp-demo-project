/**
 * @file containers_example.cpp
 * @brief Example demonstrating the usage of the containers module
 */

#include <format>
#include <print>
#include <ranges>
#include <string>
#include <vector>

#include "containers/container.hpp"

using cpp_features::containers::Container;

auto main() -> int {
  std::println("=== Containers Module Example ===");

  // Demonstrate empty container
  Container<int> empty_container;
  std::println("Empty container: {}", empty_container);

  // Demonstrate initialization with integers
  Container numbers{1, 2, 3, 4, 5};
  std::println("Integer container: {}", numbers);

  // Demonstrate adding elements
  numbers.Add(6);
  numbers.Emplace(7);
  std::println("After adding elements: {}", numbers);

  // Demonstrate views and filters
  auto even_view = numbers.GetFilteredView([](int x) { return x % 2 == 0; });
  std::println("Even numbers: {}", std::ranges::to<std::vector>(even_view));
  auto odd_view = numbers.GetFilteredView([](int x) { return x % 2 == 1; });
  std::println("Odd numbers: {}", std::ranges::to<std::vector>(odd_view));

  // Demonstrate transformation
  auto doubled_view = numbers.GetTransformedView([](int n) { return n * 2; });
  std::println("Doubled numbers: {}", std::ranges::to<std::vector>(doubled_view));

  // Demonstrate initialization with strings
  Container<std::string> words{"hello", "modern", "cpp", "world"};
  std::println("String container: {}", words);

  // Demonstrate initialization with doubles
  Container prices{19.999, 29.95, 5.50};
  std::println("Floating point container: {}", prices);

  // Demonstrate CTAD from range
  Container ctad_from_range(std::vector{10, 20, 30});
  std::println("CTAD from range: {}", ctad_from_range);

  std::println("=== Containers Module Example Completed ===\n");
  return 0;
}
