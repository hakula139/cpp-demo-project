#pragma once

#include <expected>
#include <iostream>
#include <numbers>

#include "shapes/shape.hpp"

namespace cpp_features::shapes {

class Circle final : public Shape {
 public:
  explicit Circle(double radius) : Shape("Circle"), radius_(radius) {}

  [[nodiscard]] auto GetArea() const noexcept -> std::expected<double, ShapeError> override {
    if (radius_ < 0.0) {
      return std::unexpected(ShapeError::kNegativeValue);
    }
    if (radius_ == 0.0) {
      return std::unexpected(ShapeError::kInvalidRadius);
    }
    return std::numbers::pi * radius_ * radius_;
  }

  [[nodiscard]] auto GetPerimeter() const noexcept -> std::expected<double, ShapeError> override {
    if (radius_ < 0.0) {
      return std::unexpected(ShapeError::kNegativeValue);
    }
    if (radius_ == 0.0) {
      return std::unexpected(ShapeError::kInvalidRadius);
    }
    static constexpr double K_PERIMETER_MULTIPLIER = 2.0;
    return K_PERIMETER_MULTIPLIER * std::numbers::pi *
           radius_;  // NOLINT(bugprone-narrowing-conversions)
  }

  void Draw() const override { std::cout << "Drawing a circle with radius " << radius_ << "\n"; }

  [[nodiscard]] constexpr auto GetRadius() const noexcept -> double { return radius_; }

  auto operator<=>(const Circle& other) const noexcept { return radius_ <=> other.radius_; }

  auto operator==(const Circle& other) const noexcept -> bool { return radius_ == other.radius_; }

 private:
  double radius_;
};

template <cpp_features::concepts::ArithmeticType T>
[[nodiscard]] auto CreateCircle(T radius) -> std::unique_ptr<Circle> {
  return std::make_unique<Circle>(static_cast<double>(radius));
}

}  // namespace cpp_features::shapes
