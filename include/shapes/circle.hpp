/**
 * @file circle.hpp
 * @brief Modern C++ circle shape implementation
 *
 * This file contains the Circle class implementation.
 */

#pragma once

#include <format>
#include <memory>
#include <string_view>

#include "concepts/arithmetic_concepts.hpp"
#include "exceptions/custom_exception.hpp"
#include "shapes/shape.hpp"

namespace cpp_features::shapes {

/**
 * @brief Circle shape implementation with radius-based geometry
 *
 * The Circle class provides a concrete implementation of the Shape interface for circular
 * geometry. It demonstrates modern C++ features including mathematical constants from std::numbers
 * and efficient geometric calculations.
 *
 * @code
 * Circle circle{5.0};
 * auto area = circle.GetArea();
 * std::println("Circle area: {:.2f}", area);
 *
 * // Using factory function
 * auto circle_ptr = CreateCircle(3.14);
 * circle_ptr->Draw();
 * @endcode
 */
class Circle final : public Shape {
 public:
  /**
   * @brief Construct a circle with the specified radius
   *
   * @param radius The radius of the circle
   * @throws ValidationException if radius is not positive
   *
   * Creates a circle with the given radius.
   *
   * @code
   * Circle circle{5.0};
   * @endcode
   */
  explicit Circle(double radius) : Shape{"Circle"}, radius_{radius} {
    if (radius <= 0.0) {
      throw exceptions::ValidationException{"Circle radius must be positive", "radius"};
    }
  }

  /**
   * @brief Calculate the area of the circle
   *
   * @return The area of the circle
   *
   * Calculates the area using the formula: π × r²
   *
   * @code
   * Circle circle{5.0};
   * auto area = circle.GetArea();  // area ≈ 78.54
   * @endcode
   */
  [[nodiscard]] auto GetArea() const noexcept -> double override;

  /**
   * @brief Calculate the perimeter of the circle
   *
   * @return The perimeter of the circle
   *
   * Calculates the perimeter using the formula: 2 × π × r
   *
   * @code
   * Circle circle{5.0};
   * auto perimeter = circle.GetPerimeter();  // perimeter ≈ 31.42
   * @endcode
   */
  [[nodiscard]] auto GetPerimeter() const noexcept -> double override;

  /**
   * @brief Draw the circle with formatted output
   *
   * Override of the base Draw() method that provides circle-specific drawing behavior.
   * Prints a message with the circle's radius.
   *
   * @code
   * Circle circle{3.5};
   * circle.Draw();  // Output: "Drawing Circle (r = 3.50)"
   * @endcode
   */
  void Draw() const override;

  /**
   * @brief Get the radius of the circle
   *
   * @return The circle's radius
   */
  [[nodiscard]] constexpr auto GetRadius() const noexcept -> double { return radius_; }

  /**
   * @brief Three-way comparison operator for circle ordering
   *
   * @param other The other circle to compare with
   * @return Comparison result based on radius
   *
   * @code
   * Circle small{2.0};
   * Circle large{5.0};
   * auto ordering = small <=> large;  // std::strong_ordering::less
   * @endcode
   */
  [[nodiscard]] auto operator<=>(const Circle &other) const noexcept {
    return GetRadius() <=> other.GetRadius();
  }

  /**
   * @brief Equality comparison operator
   *
   * @param other The other circle to compare with
   * @return Boolean indicating whether the circles have equal radius
   *
   * @code
   * Circle circle1{5.0};
   * Circle circle2{5.0};
   * bool equal = circle1 == circle2;  // true
   * @endcode
   */
  [[nodiscard]] auto operator==(const Circle &other) const noexcept -> bool {
    return GetRadius() == other.GetRadius();
  }

 private:
  double radius_;  ///< The radius of the circle
};

/**
 * @brief Factory function for creating Circle objects
 *
 * @tparam T Arithmetic type that can be converted to double
 * @param radius The radius value for the new circle
 * @return A unique pointer to the newly created circle
 * @throws ValidationException if radius is not positive
 *
 * Creates a new Circle instance using type-safe construction.
 *
 * @code
 * auto circle = CreateCircle(5.0);
 * @endcode
 */
template <concepts::ArithmeticType T>
[[nodiscard]] auto CreateCircle(T radius) -> std::unique_ptr<Circle> {
  return std::make_unique<Circle>(static_cast<double>(radius));
}

}  // namespace cpp_features::shapes

/**
 * @brief Custom formatter for Circle to work with std::format and std::print
 *
 * Provides formatting support for Circle objects, allowing them to be used directly with
 * std::format, std::print, and related formatting functions.
 *
 * The circle is formatted as a string in the format "Circle (r = radius)".
 *
 * @code
 * Circle circle{5.0};
 * std::print("{}", circle); // Prints: Circle (r = 5.00)
 * @endcode
 */
template <>
struct std::formatter<cpp_features::shapes::Circle> : std::formatter<std::string_view> {
  /**
   * @brief Format the circle for output
   *
   * @param circle The circle to format
   * @param ctx The format context
   * @return Iterator pointing past the formatted output
   */
  auto format(const cpp_features::shapes::Circle &circle, std::format_context &ctx) const {
    auto formatted = std::format("{} (r = {:.2f})", circle.GetName(), circle.GetRadius());
    return std::formatter<std::string_view>::format(formatted, ctx);
  }
};
