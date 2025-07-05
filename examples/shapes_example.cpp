/**
 * @file shapes_example.cpp
 * @brief Example demonstrating the usage of the shapes module
 */

#include <compare>
#include <memory>
#include <print>
#include <string_view>
#include <utility>
#include <vector>

#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "shapes/shape.hpp"

using cpp_features::shapes::CreateCircle;
using cpp_features::shapes::CreateRectangle;
using cpp_features::shapes::CreateSquare;
using cpp_features::shapes::Shape;

namespace {

/**
 * @brief Get a string representation of a comparison result
 *
 * @param ordering The comparison result
 * @return A string representation of the comparison result
 */
auto GetComparisonString(std::partial_ordering ordering) -> std::string_view {
  if (ordering == std::strong_ordering::less) {
    return "<";
  }
  if (ordering == std::strong_ordering::greater) {
    return ">";
  }
  if (ordering == std::strong_ordering::equal) {
    return "==";
  }
  return "?";
}

}  // namespace

auto main() -> int {
  std::println("=== Shapes Module Example ===");

  // Create shapes using factory functions
  std::println("\n1. Creating shapes:");

  auto circle = CreateCircle(5.0);
  std::println("   {}", *circle);

  auto rectangle = CreateRectangle(4.0, 3.0);
  std::println("   {}, is square: {}", *rectangle, rectangle->IsSquare());

  auto square = CreateSquare(2.8);
  std::println("   {}, is square: {}", *square, square->IsSquare());

  // Demonstrate polymorphic behavior
  std::vector<std::unique_ptr<Shape>> shapes;
  shapes.emplace_back(std::move(circle));
  shapes.emplace_back(std::move(rectangle));
  shapes.emplace_back(std::move(square));

  std::println("\n2. Polymorphic calculations:");
  for (const auto &shape : shapes) {
    shape->Draw();
    std::println("  Area: {:.2f}", shape->GetArea());
    std::println("  Perimeter: {:.2f}", shape->GetPerimeter());
  }

  // Demonstrate comparison
  std::println("\n3. Comparisons:");
  auto small_rect = CreateRectangle(2.0, 7.0);
  auto large_rect = CreateRectangle(8.0, 3.0);
  auto ordering = *small_rect <=> *large_rect;
  std::println("   {} {} {}", *small_rect, GetComparisonString(ordering), *large_rect);

  std::println("\n=== Shapes Module Example Completed ===");
  return 0;
}
