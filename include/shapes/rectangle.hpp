#pragma once

#include <algorithm>
#include <expected>
#include <iostream>

#include "shapes/shape.hpp"

namespace cpp_features::shapes {

class Rectangle final : public Shape {
 public:
  struct Dimensions {
    double width;
    double height;
  };

  explicit Rectangle(const Dimensions& dims)
      : Shape("Rectangle"), width_(dims.width), height_(dims.height) {}

  Rectangle(double width_value,
            double height_value)  // NOLINT(bugprone-easily-swappable-parameters)
      : Shape("Rectangle"), width_(width_value), height_(height_value) {}

  explicit Rectangle(concepts::ArithmeticType auto value)
    requires concepts::ArithmeticType<decltype(value)>
      : Shape("Rectangle"),
        width_(static_cast<double>(value)),
        height_(static_cast<double>(value)) {}

  [[nodiscard]] auto GetArea() const noexcept -> std::expected<double, ShapeError> override {
    if (width_ < 0.0 || height_ < 0.0) {
      return std::unexpected(ShapeError::kNegativeValue);
    }
    if (width_ == 0.0 || height_ == 0.0) {
      return std::unexpected(ShapeError::kInvalidDimensions);
    }
    return width_ * height_;  // NOLINT(bugprone-narrowing-conversions)
  }

  [[nodiscard]] auto GetPerimeter() const noexcept -> std::expected<double, ShapeError> override {
    if (width_ < 0.0 || height_ < 0.0) {
      return std::unexpected(ShapeError::kNegativeValue);
    }
    if (width_ == 0.0 || height_ == 0.0) {
      return std::unexpected(ShapeError::kInvalidDimensions);
    }
    static constexpr double K_PERIMETER_MULTIPLIER = 2.0;
    return K_PERIMETER_MULTIPLIER * (width_ + height_);  // NOLINT(bugprone-narrowing-conversions)
  }

  void Draw() const override {
    std::cout << "Drawing a rectangle " << width_ << " x " << height_ << "\n";
  }

  [[nodiscard]] constexpr auto GetWidth() const noexcept -> double { return width_; }

  [[nodiscard]] constexpr auto GetHeight() const noexcept -> double { return height_; }

  [[nodiscard]] auto IsSquare() const noexcept -> bool { return width_ == height_; }

  auto operator<=>(const Rectangle& other) const noexcept {
    auto area_comparison = (width_ * height_) <=> (other.width_ * other.height_);
    if (area_comparison != std::strong_ordering::equal) {
      return area_comparison;
    }
    auto width_comparison = width_ <=> other.width_;
    if (width_comparison != std::strong_ordering::equal) {
      return width_comparison;
    }
    return height_ <=> other.height_;
  }

  auto operator==(const Rectangle& other) const noexcept -> bool {
    return width_ == other.width_ && height_ == other.height_;
  }

 private:
  double width_;
  double height_;
};

template <cpp_features::concepts::ArithmeticType T, cpp_features::concepts::ArithmeticType U>
[[nodiscard]] auto CreateRectangle(T width, U height) -> std::unique_ptr<Rectangle> {
  return std::make_unique<Rectangle>(static_cast<double>(width), static_cast<double>(height));
}

template <cpp_features::concepts::ArithmeticType T>
[[nodiscard]] auto CreateSquare(T side) -> std::unique_ptr<Rectangle> {
  return std::make_unique<Rectangle>(static_cast<double>(side), static_cast<double>(side));
}

}  // namespace cpp_features::shapes
