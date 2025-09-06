/**
 * @file shapes_example.cpp
 * @brief Example demonstrating the usage of the shapes module
 */

#include <compare>
#include <memory>
#include <print>
#include <string_view>
#include <vector>

#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "shapes/shape.hpp"

using cpp_features::shapes::CreateCircle;
using cpp_features::shapes::CreateRectangle;
using cpp_features::shapes::CreateShape;
using cpp_features::shapes::CreateSquare;
using cpp_features::shapes::Rectangle;
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

/**
 * @brief Demonstrate basic shape creation using factory functions and constructors
 */
void DemonstrateShapeCreation() {
  std::println("\n1. Creating shapes:");

  // Create a circle using factory function
  auto circle = CreateCircle(5.0);
  std::println("   {}", *circle);

  // Create a rectangle using factory function from shapes module
  auto rectangle = CreateShape<Rectangle>(4.0, 3.0);
  std::println("   {}, is square: {}", *rectangle, rectangle->IsSquare());

  // Create a square using constructor
  Rectangle square{2.5};
  std::println("   {}, is square: {}", square, square.IsSquare());
}

/**
 * @brief Demonstrate polymorphic behavior with shape calculations
 */
void DemonstratePolymorphicBehavior() {
  std::println("\n2. Polymorphic calculations:");

  std::vector<std::unique_ptr<Shape>> shapes;
  shapes.emplace_back(CreateCircle(5.0));
  shapes.emplace_back(CreateRectangle(4.0, 3.0));
  shapes.emplace_back(CreateSquare(2.5));

  for (const auto &shape : shapes) {
    shape->Draw();
    auto area = shape->GetArea();
    auto perimeter = shape->GetPerimeter();
    std::println("   Area: {:.2f}, Perimeter: {:.2f}", area, perimeter);
  }
}

/**
 * @brief Demonstrate shape comparison operations
 */
void DemonstrateShapeComparisons() {
  std::println("\n3. Comparisons:");

  auto rect1 = CreateRectangle(2.0, 7.0);
  auto rect2 = CreateRectangle(8.0, 3.0);
  auto ordering12 = *rect1 <=> *rect2;
  auto equal12 = *rect1 == *rect2;
  std::println("   {} {} {} (equal: {})", *rect1, GetComparisonString(ordering12), *rect2, equal12);

  auto rect3 = CreateRectangle(4.0, 6.0);
  auto ordering23 = *rect2 <=> *rect3;
  auto equal23 = *rect2 == *rect3;
  std::println("   {} {} {} (equal: {})", *rect2, GetComparisonString(ordering23), *rect3, equal23);

  auto rect4 = CreateRectangle(8.0, 3.0);
  auto ordering24 = *rect2 <=> *rect4;
  auto equal24 = *rect2 == *rect4;
  std::println("   {} {} {} (equal: {})", *rect2, GetComparisonString(ordering24), *rect4, equal24);
}

}  // namespace

auto main() -> int {
  std::println("=== Shapes Module Example ===");

  DemonstrateShapeCreation();
  DemonstratePolymorphicBehavior();
  DemonstrateShapeComparisons();

  std::println("\n=== Shapes Module Example Completed ===");
  return 0;
}
