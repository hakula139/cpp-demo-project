/**
 * @file utility_concepts.hpp
 * @brief Utility concepts for common C++ template constraints
 *
 * This file contains concept definitions for common utility operations and type
 * properties, providing compile-time constraints for template parameters that
 * need specific capabilities like printing, comparison, construction, and copying.
 */

#pragma once

#include <concepts>
#include <ostream>

namespace cpp_features::concepts {

/**
 * @brief Concept for types that can be printed to output streams
 *
 * @tparam T The type to check for printability
 *
 * This concept ensures that a type supports the stream insertion operator (<<)
 * with std::ostream, making it suitable for printing and logging operations.
 * The operator must return a reference to std::ostream to support chaining.
 *
 * @code
 * template <PrintableType T>
 * void Print(const T &value) {
 *   std::cout << value << std::endl;
 * }
 * @endcode
 */
template <typename T>
concept PrintableType = requires(T value, std::ostream &os) {
  { os << value } -> std::convertible_to<std::ostream &>;
};

/**
 * @brief Concept for types that support total ordering
 *
 * @tparam T The type to check for comparison support
 *
 * This concept ensures that a type supports all comparison operations
 * (==, !=, <, <=, >, >=) and that these operations form a total ordering.
 * It leverages the standard library's std::totally_ordered concept.
 *
 * @code
 * template <ComparableType T>
 * auto FindMax(const std::vector<T> &values) -> T {
 *   return *std::max_element(values.begin(), values.end());
 * }
 * @endcode
 */
template <typename T>
concept ComparableType = std::totally_ordered<T>;

/**
 * @brief Concept for types that can be default-constructed
 *
 * @tparam T The type to check for default construction support
 *
 * This concept ensures that a type can be constructed without arguments,
 * either through a default constructor or by value initialization.
 * It leverages the standard library's std::default_initializable concept.
 *
 * @code
 * template <DefaultConstructibleType T>
 * auto CreateDefaultVector(std::size_t count) -> std::vector<T> {
 *   return std::vector<T>(count);  // Creates count default-constructed elements
 * }
 * @endcode
 */
template <typename T>
concept DefaultConstructibleType = std::default_initializable<T>;

/**
 * @brief Concept for types that support copying
 *
 * @tparam T The type to check for copy support
 *
 * This concept ensures that a type can be copied through copy construction
 * and copy assignment. It leverages the standard library's std::copyable concept,
 * which also requires the type to be movable and default constructible.
 *
 * @code
 * template <CopyableType T>
 * auto DuplicateElements(const std::vector<T> &source) -> std::vector<T> {
 *   std::vector<T> result;
 *   for (const auto &element : source) {
 *     result.push_back(element);  // Requires copy construction
 *   }
 *   return result;
 * }
 * @endcode
 */
template <typename T>
concept CopyableType = std::copyable<T>;

/**
 * @brief Concept for types that support moving
 *
 * @tparam T The type to check for move support
 *
 * This concept ensures that a type can be moved through move construction
 * and move assignment. It leverages the standard library's std::movable concept,
 * which also requires the type to be default constructible and swappable.
 *
 * @code
 * template <MovableType T>
 * void TransferOwnership(std::vector<T> &source, std::vector<T> &destination) {
 *   for (auto &element : source) {
 *     destination.push_back(std::move(element));  // Requires move construction
 *   }
 *   source.clear();
 * }
 * @endcode
 */
template <typename T>
concept MovableType = std::movable<T>;

}  // namespace cpp_features::concepts
