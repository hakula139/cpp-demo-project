#include "shapes/rectangle.hpp"

#include <compare>
#include <print>

namespace cpp_features::shapes {

[[nodiscard]] auto Rectangle::GetArea() const noexcept -> double {
  return GetWidth() * GetHeight();
}

[[nodiscard]] auto Rectangle::GetPerimeter() const noexcept -> double {
  return 2.0 * (GetWidth() + GetHeight());
}

void Rectangle::Draw() const {
  std::println("Drawing a rectangle (w = {:.2f}, h = {:.2f})", GetWidth(), GetHeight());
}

auto Rectangle::operator<=>(const Rectangle &other) const noexcept {
  if (auto area_cmp = GetArea() <=> other.GetArea(); area_cmp != std::strong_ordering::equal) {
    return area_cmp;
  }
  return GetWidth() <=> other.GetWidth();
}

}  // namespace cpp_features::shapes
