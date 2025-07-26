/**
 * @file rectangle.hpp
 * @brief Modern C++ rectangle shape implementation
 *
 * This file contains the Rectangle class implementation.
 */

#pragma once

#include <compare>
#include <format>
#include <memory>
#include <string_view>

#include "concepts/arithmetic_concepts.hpp"
#include "exceptions/custom_exception.hpp"
#include "shapes/shape.hpp"

namespace cpp_features::shapes {

/**
 * @brief Rectangle shape implementation with width and height geometry
 *
 * The Rectangle class provides a concrete implementation of the Shape interface for rectangular
 * geometry. It demonstrates modern C++ features including flexible constructor patterns and
 * efficient geometric calculations.
 *
 * @code
 * Rectangle rect{4.0, 3.0};
 * auto area = rect.GetArea();
 * std::println("Rectangle area: {:.2f}", area);
 *
 * // Using factory function
 * auto rect_ptr = CreateRectangle(5.0, 7.0);
 * rect_ptr->Draw();
 * @endcode
 */
class Rectangle final : public Shape {
 public:
  /**
   * @brief Dimensions structure for rectangle construction
   *
   * A simple structure to hold width and height dimensions for rectangle construction.
   * This provides a convenient way to pass dimensions as a single parameter.
   *
   * @code
   * Rectangle::Dimensions dimensions{.width = 4.0, .height = 3.0};
   * @endcode
   */
  struct Dimensions {
    double width;   ///< Width of the rectangle
    double height;  ///< Height of the rectangle
  };

  /**
   * @brief Construct a rectangle from a Dimensions structure
   *
   * @param dimensions Dimensions structure containing width and height
   * @throws ValidationException if width or height is not positive
   *
   * Creates a rectangle using the provided dimensions structure.
   *
   * @code
   * Rectangle::Dimensions dimensions{.width = 4.0, .height = 3.0};
   * Rectangle rect{dimensions};
   * @endcode
   */
  explicit Rectangle(Dimensions dimensions) : Shape{"Rectangle"}, dimensions_{dimensions} {
    if (dimensions.width <= 0.0) {
      throw exceptions::ValidationException{"Rectangle dimensions must be positive", "width"};
    }
    if (dimensions.height <= 0.0) {
      throw exceptions::ValidationException{"Rectangle dimensions must be positive", "height"};
    }
  }

  /**
   * @brief Construct a rectangle with separate width and height values
   *
   * @param width Width of the rectangle
   * @param height Height of the rectangle
   * @throws ValidationException if width or height is not positive
   *
   * Creates a rectangle with the specified width and height.
   *
   * @code
   * Rectangle rect{4.0, 3.0};
   * @endcode
   */
  Rectangle(double width, double height)
      : Rectangle{Dimensions{.width = width, .height = height}} {}

  /**
   * @brief Construct a square rectangle with equal width and height
   *
   * @param side Side length for the square (both width and height)
   * @throws ValidationException if side length is not positive
   *
   * Creates a square rectangle where width equals height.
   *
   * @code
   * Rectangle square{5.0};  // 5.0 x 5.0 square
   * @endcode
   */
  explicit Rectangle(double side) : Rectangle{Dimensions{.width = side, .height = side}} {}

  /**
   * @brief Calculate the area of the rectangle
   *
   * @return The area of the rectangle
   *
   * Calculates the area using the formula: width × height
   *
   * @code
   * Rectangle rect{4.0, 3.0};
   * auto area = rect.GetArea();  // area = 12.0
   * @endcode
   */
  [[nodiscard]] auto GetArea() const noexcept -> double override;

  /**
   * @brief Calculate the perimeter of the rectangle
   *
   * @return The perimeter of the rectangle
   *
   * Calculates the perimeter using the formula: 2 × (width + height)
   *
   * @code
   * Rectangle rect{4.0, 3.0};
   * auto perimeter = rect.GetPerimeter();  // perimeter = 14.0
   * @endcode
   */
  [[nodiscard]] auto GetPerimeter() const noexcept -> double override;

  /**
   * @brief Draw the rectangle with formatted output
   *
   * Override of the base Draw() method that provides rectangle-specific drawing behavior.
   * Prints a message with the rectangle's dimensions.
   *
   * @code
   * Rectangle rect{4.0, 3.0};
   * rect.Draw();  // Output: "Drawing Rectangle (w = 4.00, h = 3.00)"
   * @endcode
   */
  void Draw() const override;

  /**
   * @brief Get the width of the rectangle
   *
   * @return The rectangle's width
   */
  [[nodiscard]] constexpr auto GetWidth() const noexcept -> double { return dimensions_.width; }

  /**
   * @brief Get the height of the rectangle
   *
   * @return The rectangle's height
   */
  [[nodiscard]] constexpr auto GetHeight() const noexcept -> double { return dimensions_.height; }

  /**
   * @brief Check if the rectangle is a square
   *
   * @return Boolean indicating whether the rectangle is a square
   *
   * Determines if the rectangle is a square by comparing width and height for exact equality.
   * Returns true if both dimensions are exactly equal, false otherwise.
   *
   * @code
   * Rectangle rect{4.0, 3.0};
   * Rectangle square{5.0, 5.0};
   * bool is_rect_square = rect.IsSquare();      // false
   * bool is_square_square = square.IsSquare();  // true
   * @endcode
   */
  [[nodiscard]] auto IsSquare() const noexcept -> bool {
    return dimensions_.width == dimensions_.height;
  }

  /**
   * @brief Three-way comparison operator for rectangle ordering
   *
   * @param other The other rectangle to compare with
   * @return Comparison result based on area, then width
   *
   * Compares rectangles using a hierarchical approach:
   *
   * 1. First by area (width × height)
   * 2. If areas are equal, then by width
   *
   * @code
   * Rectangle small{2.0, 3.0};        // area = 6.0
   * Rectangle large{4.0, 5.0};        // area = 20.0
   * auto ordering = small <=> large;  // std::strong_ordering::less
   * @endcode
   */
  [[nodiscard]] auto operator<=>(const Rectangle &other) const noexcept {
    if (auto area_cmp = GetArea() <=> other.GetArea(); area_cmp != std::strong_ordering::equal) {
      return area_cmp;
    }
    return GetWidth() <=> other.GetWidth();
  }

  /**
   * @brief Equality comparison operator
   *
   * @param other The other rectangle to compare with
   * @return Boolean indicating whether the rectangles have equal dimensions
   *
   * Compares rectangles for equality based on their width and height values.
   * Two rectangles are considered equal if both width and height are exactly equal.
   *
   * @code
   * Rectangle rect1{4.0, 3.0};
   * Rectangle rect2{4.0, 3.0};
   * bool equal = rect1 == rect2;  // true
   * @endcode
   */
  auto operator==(const Rectangle &other) const noexcept -> bool {
    return GetWidth() == other.GetWidth() && GetHeight() == other.GetHeight();
  }

 private:
  Dimensions dimensions_;
};

/**
 * @brief Factory function for creating Rectangle objects
 *
 * @tparam T1 Arithmetic type that can be converted to double (width)
 * @tparam T2 Arithmetic type that can be converted to double (height)
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @return A unique pointer to the newly created rectangle
 * @throws ValidationException if width or height is not positive
 *
 * Creates a new Rectangle instance using type-safe construction.
 * Supports mixed arithmetic types for width and height.
 *
 * @code
 * auto rect = CreateRectangle(4.0, 3.0);
 * @endcode
 */
template <concepts::ArithmeticType T1, concepts::ArithmeticType T2>
[[nodiscard]] auto CreateRectangle(T1 width, T2 height) -> std::unique_ptr<Rectangle> {
  return std::make_unique<Rectangle>(static_cast<double>(width), static_cast<double>(height));
}

/**
 * @brief Factory function for creating square Rectangle objects
 *
 * @tparam T Arithmetic type that can be converted to double
 * @param side Side length for the square (both width and height)
 * @return A unique pointer to the newly created square rectangle
 * @throws ValidationException if side length is not positive
 *
 * Creates a new square Rectangle instance where width equals height.
 *
 * @code
 * auto square = CreateSquare(5.0);
 * @endcode
 */
template <concepts::ArithmeticType T>
[[nodiscard]] auto CreateSquare(T side) -> std::unique_ptr<Rectangle> {
  return std::make_unique<Rectangle>(static_cast<double>(side));
}

}  // namespace cpp_features::shapes

/**
 * @brief Custom formatter for Rectangle to work with std::format and std::print
 *
 * Provides formatting support for Rectangle objects, allowing them to be used directly with
 * std::format, std::print, and related formatting functions.
 *
 * The rectangle is formatted as a string in the format "Rectangle (w = width, h = height)".
 *
 * @code
 * Rectangle rect{4.0, 3.0};
 * std::print("{}", rect);  // Prints: Rectangle (w = 4.00, h = 3.00)
 *
 * Rectangle square{5.0};
 * std::print("{}", square);  // Prints: Rectangle (w = 5.00, h = 5.00)
 * @endcode
 */
template <>
struct std::formatter<cpp_features::shapes::Rectangle> : std::formatter<std::string_view> {
  /**
   * @brief Format the rectangle for output
   *
   * @param rect The rectangle to format
   * @param ctx The format context
   * @return Iterator pointing past the formatted output
   */
  auto format(const cpp_features::shapes::Rectangle &rect, std::format_context &ctx) const {
    auto formatted = std::format("{} (w = {:.2f}, h = {:.2f})", rect.GetName(), rect.GetWidth(),
                                 rect.GetHeight());
    return std::formatter<std::string_view>::format(formatted, ctx);
  }
};
