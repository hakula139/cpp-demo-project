#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <chrono>
#include <thread>
#include <ranges>
#include <print>
#include <expected>
#include <format>

#include "shapes/shape.hpp"
#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "containers/container.hpp"
#include "concepts/arithmetic_concepts.hpp"

using namespace cpp_features;

template<concepts::ArithmeticType T>
[[nodiscard]] constexpr auto Maximum(T a, T b) noexcept -> T {
  return (a > b) ? a : b;
}

void DemonstrateRanges() {
  std::print("\n=== C++23 Ranges Demonstration ===\n");

  std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6, 10};

  std::print("Original vector: ");
  for (const auto& n : numbers) {
    std::print("{} ", n);
  }
  std::print("\n");

  auto even_squared = numbers
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; });

  std::print("Even numbers squared: ");
  for (const auto& n : even_squared) {
    std::print("{} ", n);
  }
  std::print("\n");

  auto sorted_numbers = numbers;
  std::ranges::sort(sorted_numbers);

  std::print("Sorted numbers: ");
  std::ranges::for_each(sorted_numbers, [](int n) {
    std::print("{} ", n);
  });
  std::print("\n");
}

void DemonstrateExpected() {
  std::print("\n=== C++23 std::expected Demonstration ===\n");

  auto circle = shapes::CreateCircle(5.0);
  auto rectangle = shapes::CreateRectangle(4.0, 6.0);

  std::vector<std::unique_ptr<shapes::Shape>> shape_collection;
  shape_collection.push_back(std::move(circle));
  shape_collection.push_back(std::move(rectangle));

  for (const auto& shape : shape_collection) {
    shape->Draw();

    auto area_result = shape->GetArea();
    if (area_result) {
      std::print("Area: {:.2f}\n", area_result.value());
    } else {
      std::print("Error calculating area: {}\n",
                static_cast<int>(area_result.error()));
    }

    auto perimeter_result = shape->GetPerimeter();
    if (perimeter_result) {
      std::print("Perimeter: {:.2f}\n", perimeter_result.value());
    } else {
      std::print("Error calculating perimeter: {}\n",
                static_cast<int>(perimeter_result.error()));
    }
    std::print("\n");
  }
}

void DemonstrateSmartPointers() {
  std::print("\n=== Smart Pointers Demonstration ===\n");

  auto unique_circle = shapes::CreateCircle(3.0);
  unique_circle->Draw();

  auto area = unique_circle->GetArea();
  if (area) {
    std::print("Circle area: {:.2f}\n", area.value());
  }

  auto shared_rect = std::make_shared<shapes::Rectangle>(4.0, 5.0);
  auto shared_rect2 = shared_rect;  // Shared ownership

  shared_rect->Draw();
  auto rect_area = shared_rect->GetArea();
  if (rect_area) {
    std::print("Rectangle area: {:.2f}\n", rect_area.value());
  }
  std::print("Reference count: {}\n", shared_rect.use_count());
}

void DemonstrateRandom() {
  std::print("\n=== Random Number Generation ===\n");

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> int_dist(1, 100);
  std::uniform_real_distribution<> real_dist(0.0, 10.0);

  std::print("Random integers: ");
  for (int i = 0; i < 10; ++i) {
    std::print("{} ", int_dist(gen));
  }
  std::print("\n");

  std::print("Random reals: ");
  for (int i = 0; i < 5; ++i) {
    std::print("{:.2f} ", real_dist(gen));
  }
  std::print("\n");
}

void DemonstrateContainers() {
  std::print("\n=== Modern Container Demonstration ===\n");

  containers::Container<int> int_container{10, 20, 30, 40, 50};
  int_container.Display();

  containers::Container<std::string> string_container;
  string_container.Add("Hello");
  string_container.Add("Modern");
  string_container.Add("C++23");
  string_container.Display();

  auto filtered_view = int_container.GetFilteredView([](int n) {
    return n > 25;
  });

  std::print("Filtered view (> 25): ");
  for (const auto& item : filtered_view) {
    std::print("{} ", item);
  }
  std::print("\n");

  auto transformed_view = int_container.GetTransformedView([](int n) {
    return n * 2;
  });

  std::print("Transformed view (* 2): ");
  for (const auto& item : transformed_view) {
    std::print("{} ", item);
  }
  std::print("\n");
}

template<concepts::NumericType T>
auto CalculateStatistics(const std::vector<T>& data) {
  if (data.empty()) {
    return std::expected<std::pair<T, T>, std::string>{
      std::unexpected("Empty data set")
    };
  }

  T sum = std::accumulate(data.begin(), data.end(), T{});
  T mean = sum / static_cast<T>(data.size());

  auto sorted_data = data;
  std::ranges::sort(sorted_data);
  T median = sorted_data[sorted_data.size() / 2];

  return std::expected<std::pair<T, T>, std::string>{
    std::make_pair(mean, median)
  };
}

void DemonstrateConcepts() {
  std::print("\n=== C++23 Concepts Demonstration ===\n");

  std::vector<double> values = {1.5, 2.7, 3.2, 4.8, 5.1, 6.9, 7.3, 8.4, 9.6};

  auto stats = CalculateStatistics(values);
  if (stats) {
    auto [mean, median] = stats.value();
    std::print("Mean: {:.2f}, Median: {:.2f}\n", mean, median);
  } else {
    std::print("Error: {}\n", stats.error());
  }

  std::print("Maximum of 15 and 25: {}\n", Maximum(15, 25));
  std::print("Maximum of 3.14 and 2.71: {:.2f}\n", Maximum(3.14, 2.71));
}

void DemonstrateTiming() {
  std::print("\n=== Timing Demonstration ===\n");

  auto start = std::chrono::high_resolution_clock::now();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::print("Execution time: {} milliseconds\n", duration.count());
}

int main() {
  std::print("=== Modern C++23 Feature Testing Program ===\n");
  std::print("Following Google C++ Style Guide\n");

  DemonstrateRanges();
  DemonstrateExpected();
  DemonstrateSmartPointers();
  DemonstrateRandom();
  DemonstrateContainers();
  DemonstrateConcepts();
  DemonstrateTiming();

  std::print("\nProgram completed successfully!\n");
  return 0;
}