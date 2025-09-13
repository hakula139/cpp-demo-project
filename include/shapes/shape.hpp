/**
 * @file shape.hpp
 * @brief Modern C++ shape hierarchy with concepts and type-safe factory functions
 *
 * This file contains the base Shape class and related utilities for a polymorphic shape hierarchy.
 * The module demonstrates modern C++ features including concepts and type-safe factory functions.
 */

#pragma once

#include <concepts>
#include <memory>
#include <print>
#include <string>
#include <string_view>

#include "concepts/arithmetic_concepts.hpp"

namespace cpp_features::shapes {

/**
 * @brief Abstract base class for geometric shapes
 *
 * The Shape class provides a polymorphic interface for geometric shapes with constructor-based
 * validation and modern C++ features. It serves as the foundation for a shape hierarchy that
 * demonstrates object-oriented design principles and modern C++ best practices.
 *
 * All derived classes must implement GetArea() and GetPerimeter() methods.
 *
 * @code
 * class Circle : public Shape {
 *  public:
 *   explicit Circle(double radius) : Shape{"Circle"}, radius_{radius} {}
 *
 *   [[nodiscard]] auto GetArea() const noexcept -> double override {
 *     return std::numbers::pi * radius_ * radius_;
 *   }
 *
 *   // ... other methods
 *
 *  private:
 *   double radius_;  ///< The radius of the circle
 * };
 * @endcode
 */
class Shape {
 public:
  /**
   * @brief Virtual destructor for proper polymorphic cleanup
   *
   * Ensures proper cleanup of derived classes when destroyed through base class pointers.
   * This is essential for polymorphic behavior and preventing resource leaks.
   */
  virtual ~Shape() = default;

  /**
   * @brief Copy constructor
   */
  Shape(const Shape &) = default;

  /**
   * @brief Copy assignment operator
   */
  auto operator=(const Shape &) -> Shape & = default;

  /**
   * @brief Move constructor
   */
  Shape(Shape &&) noexcept = default;

  /**
   * @brief Move assignment operator
   */
  auto operator=(Shape &&) noexcept -> Shape & = default;

  /**
   * @brief Calculate the area of the shape
   *
   * @return Area of the shape
   *
   * Pure virtual function that must be implemented by all derived classes.
   * Returns the area of the shape.
   */
  [[nodiscard]] virtual auto GetArea() const noexcept -> double = 0;

  /**
   * @brief Calculate the perimeter of the shape
   *
   * @return Perimeter of the shape
   *
   * Pure virtual function that must be implemented by all derived classes.
   * Returns the perimeter of the shape.
   */
  [[nodiscard]] virtual auto GetPerimeter() const noexcept -> double = 0;

  /**
   * @brief Draw the shape (virtual method for polymorphic behavior)
   *
   * Virtual method that provides a default implementation for drawing the shape.
   * Derived classes can override this method to provide specific drawing behavior.
   * The default implementation prints a generic message.
   */
  virtual void Draw() const { std::println("Drawing {}", GetName()); }

  /**
   * @brief Get the name of the shape
   *
   * @return A view of the shape's name
   */
  [[nodiscard]] constexpr auto GetName() const noexcept -> std::string_view { return name_; }

 protected:
  /**
   * @brief Construct a shape with the specified name
   *
   * @param name Name identifying the type of shape (e.g., "Circle", "Rectangle")
   *
   * Creates a shape with the given name. Protected constructor ensures this class can only be
   * instantiated through derived classes.
   */
  explicit Shape(std::string_view name) : name_{name} {}

 private:
  std::string name_;  ///< Name of the shape for identification and display purposes
};

/**
 * @brief Type-safe factory function for creating shapes
 *
 * @tparam ShapeType The specific shape type to create (must derive from Shape)
 * @tparam Args Types of constructor arguments
 * @param args Arguments to forward to the shape constructor
 * @return A unique pointer to the newly created shape
 *
 * Creates a new shape instance using perfect forwarding and type safety through concepts.
 *
 * @code
 * auto circle = CreateShape<Circle>(5.0);
 * auto rectangle = CreateShape<Rectangle>(3.0, 4.0);
 * @endcode
 */
template <typename ShapeType, concepts::ArithmeticType... Args>
  requires std::derived_from<ShapeType, Shape>
[[nodiscard]] auto CreateShape(Args &&...args) -> std::unique_ptr<ShapeType> {
  return std::make_unique<ShapeType>(std::forward<Args>(args)...);
}

}  // namespace cpp_features::shapes
