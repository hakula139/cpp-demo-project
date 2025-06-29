/**
 * @file container_concepts.hpp
 * @brief Container-related concepts for C++ template constraints
 *
 * This file contains concept definitions for container types, providing
 * compile-time constraints for template parameters that need to work with
 * container-like objects and ranges.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>

namespace cpp_features::concepts {

/**
 * @brief Concept for iterable container types
 *
 * @tparam Container The container type to check for iterability
 *
 * This concept ensures that a container type provides the basic iteration interface
 * required for range-based for loops and STL algorithms. It requires:
 *
 * - begin() method that returns an input or output iterator
 * - end() method that returns an input or output iterator
 * - size() method that returns a value convertible to std::size_t
 *
 * This concept is useful for constraining template parameters that need to iterate
 * over container elements or query container size.
 *
 * @code
 * template <IterableContainer Container>
 * void PrintElements(const Container &container) {
 *   std::println("Size: {}", container.size());
 *   for (auto it = container.begin(); it != container.end(); ++it) {
 *     std::println("{}", *it);
 *   }
 * }
 * @endcode
 */
template <typename Container>
concept IterableContainer = requires(Container c) {
  { c.begin() } -> std::input_or_output_iterator;
  { c.end() } -> std::input_or_output_iterator;
  { c.size() } -> std::convertible_to<std::size_t>;
};

/**
 * @brief Concept for range-compatible container types
 *
 * @tparam Container The container type to check for range compatibility
 *
 * This concept ensures that a container type is compatible with C++20 ranges
 * by satisfying the std::ranges::range concept. This is a more modern and
 * comprehensive approach to container constraints, as it leverages the
 * standard library's range concepts.
 *
 * Types that satisfy this concept can be used with:
 *
 * - Range-based for loops
 * - C++20 ranges algorithms
 * - Views and range adaptors
 *
 * @code
 * template <RangeContainer Container>
 * auto ProcessRange(Container &&container) {
 *   return container | std::views::filter([](const auto &element) { return element > 0; })
 *                    | std::views::transform([](const auto &element) { return element * 2; });
 * }
 * @endcode
 */
template <typename Container>
concept RangeContainer = std::ranges::range<Container>;

}  // namespace cpp_features::concepts
