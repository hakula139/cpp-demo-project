/**
 * @file arithmetic_concepts.hpp
 * @brief Arithmetic type concepts for C++ template constraints
 *
 * This file contains concept definitions for arithmetic operations and types,
 * providing compile-time constraints for template parameters that need to
 * support arithmetic operations.
 */

#pragma once

#include <concepts>

namespace cpp_features::concepts {

/**
 * @brief Concept for arithmetic types
 *
 * @tparam T The type to check for arithmetic properties
 *
 * This concept ensures that a type is either an integral type or a floating-point type.
 * It combines the standard library concepts std::integral and std::floating_point
 * to provide a unified constraint for all arithmetic types.
 *
 * @code
 * template <ArithmeticType T>
 * auto AddNumbers(T first_value, T second_value) -> T {
 *   return first_value + second_value;
 * }
 * @endcode
 */
template <typename T>
concept ArithmeticType = std::integral<T> || std::floating_point<T>;

/**
 * @brief Concept for types that support addition
 *
 * @tparam T The type to check for addition support
 *
 * This concept ensures that a type supports the addition operator (+) and that
 * the result of the addition operation is of the same type T. This is useful
 * for constraining template parameters that need to perform addition operations.
 *
 * @code
 * template <AddableType T>
 * auto SumValues(const std::vector<T> &values) -> T {
 *   T result{};
 *   for (const auto &value : values) {
 *     result += value;
 *   }
 *   return result;
 * }
 * @endcode
 */
template <typename T>
concept AddableType = requires(T lhs, T rhs) {
  { lhs + rhs } -> std::same_as<T>;
};

/**
 * @brief Concept for types that support subtraction
 *
 * @tparam T The type to check for subtraction support
 *
 * This concept ensures that a type supports the subtraction operator (-) and that
 * the result of the subtraction operation is of the same type T. This is useful
 * for constraining template parameters that need to perform subtraction operations.
 *
 * @code
 * template <SubtractableType T>
 * auto CalculateDifference(T first_value, T second_value) -> T {
 *   return first_value - second_value;
 * }
 * @endcode
 */
template <typename T>
concept SubtractableType = requires(T lhs, T rhs) {
  { lhs - rhs } -> std::same_as<T>;
};

/**
 * @brief Concept for complete numeric types
 *
 * @tparam T The type to check for numeric properties
 *
 * This concept combines ArithmeticType, AddableType, and SubtractableType to ensure
 * that a type is a complete numeric type that supports all basic arithmetic operations.
 * This is the most comprehensive arithmetic concept and should be used when full
 * numeric functionality is required.
 *
 * @code
 * template <NumericType T>
 * auto CalculateAverage(const std::vector<T> &values) -> T {
 *   if (values.empty()) {
 *     return T{};
 *   }
 *   T sum{};
 *   for (const auto &value : values) {
 *     sum += value;
 *   }
 *   return sum / values.size();
 * }
 * @endcode
 */
template <typename T>
concept NumericType = ArithmeticType<T> && AddableType<T> && SubtractableType<T>;

}  // namespace cpp_features::concepts
