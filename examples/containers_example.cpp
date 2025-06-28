/**
 * @file containers_example.cpp
 * @brief Example demonstrating the usage of the containers module
 */

#include <algorithm>
#include <cstddef>
#include <format>
#include <print>
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
  Container<int> numbers{1, 2, 3, 4, 5};
  std::println("Integer container: {}", numbers);

  // Demonstrate adding elements
  numbers.Add(6);
  numbers.Add(7);
  std::println("After adding elements: {}", numbers);

  // Demonstrate removing elements
  auto value = 3;
  auto removed_count = numbers.Remove(value);
  std::println("Removed {} elements of value {}", removed_count, value);
  std::println("After removing elements: {}", numbers);

  // Demonstrate safe access to elements
  std::size_t index = 3;
  auto result = numbers.At(index);
  if (result) {
    std::println("Element at index {}: {}", index, result->get());
  } else {
    std::println("Error accessing element at index {}", index);
  }

  // Demonstrate views and filters
  auto even_view = numbers.GetFilteredView([](int x) { return x % 2 == 0; });
  std::print("Even numbers: ");
  std::ranges::for_each(even_view, [](int x) { std::print("{} ", x); });
  std::println();

  // Demonstrate transformation
  auto doubled_view = numbers.GetTransformedView([](int n) { return n * 2; });
  std::print("Doubled numbers: ");
  std::ranges::for_each(doubled_view, [](int x) { std::print("{} ", x); });
  std::println();

  // Demonstrate initialization with strings
  Container<std::string> words{"hello", "modern", "cpp", "world"};
  std::println("String container: {}", words);

  // Demonstrate emplacing elements
  words.Emplace(5, '!');
  std::println("After emplacing elements: {}", words);

  // Demonstrate initialization with doubles
  Container<double> prices{19.999, 29.95, 5.50};
  std::println("Floating point container: {}", prices);

  // Demonstrate CTAD from range
  std::vector source{10, 20, 30};
  Container container(source);
  std::println("CTAD from range: {}", container);

  std::println("=== Containers Module Example Completed ===");
  return 0;
}
