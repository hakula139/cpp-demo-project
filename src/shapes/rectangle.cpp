#include "shapes/rectangle.hpp"

#include <print>

namespace cpp_features::shapes {

[[nodiscard]] auto Rectangle::GetArea() const noexcept -> double {
  return GetWidth() * GetHeight();
}

[[nodiscard]] auto Rectangle::GetPerimeter() const noexcept -> double {
  return 2.0 * (GetWidth() + GetHeight());
}

void Rectangle::Draw() const { std::println("Drawing {}", *this); }

}  // namespace cpp_features::shapes
