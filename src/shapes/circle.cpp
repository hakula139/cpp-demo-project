#include "shapes/circle.hpp"

#include <numbers>
#include <print>

namespace cpp_features::shapes {

[[nodiscard]] auto Circle::GetArea() const noexcept -> double {
  return std::numbers::pi * GetRadius() * GetRadius();
}

[[nodiscard]] auto Circle::GetPerimeter() const noexcept -> double {
  return 2.0 * std::numbers::pi * GetRadius();
}

void Circle::Draw() const { std::println("Drawing a circle (r = {:.2f})", GetRadius()); }

}  // namespace cpp_features::shapes
