/**
 * @file stl.hpp
 * @brief Modern C++ STL algorithm wrappers with concepts and ranges support
 *
 * This file contains modern C++ algorithm implementations that demonstrate
 * the use of concepts, ranges, and STL algorithms. The functions provide
 * type-safe wrappers around standard algorithms with enhanced functionality.
 */

#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

#include "concepts/container_concepts.hpp"

namespace cpp_features::algorithms {

/**
 * @brief Sort a container using std::ranges::sort
 *
 * @tparam Container Type of the container to sort. Must satisfy SortableContainer concept.
 * @param container The container to sort in-place
 *
 * Sorts the elements in the container in ascending order using the default comparison operator.
 * The container must satisfy the SortableContainer concept, which ensures it has the necessary
 * properties for sorting operations.
 *
 * @code
 * std::vector<int> numbers{3, 1, 4, 1, 5, 9};
 * SortContainer(numbers);
 * // numbers == {1, 1, 3, 4, 5, 9}
 * @endcode
 */
template <concepts::SortableContainer Container>
void SortContainer(Container &container) {
  std::ranges::sort(container);
}

/**
 * @brief Count elements in a range that satisfy a predicate
 *
 * @tparam Range Type of the input range. Must satisfy std::ranges::input_range.
 * @tparam Predicate Type of the predicate function
 * @param range The input range to examine
 * @param predicate The predicate function to test each element
 * @return The number of elements that satisfy the predicate
 *
 * Counts the number of elements in the range for which the predicate returns true.
 * The predicate must be callable with elements of the range and return a value
 * convertible to bool.
 *
 * @code
 * std::vector<int> numbers{1, 2, 3, 4, 5, 6};
 * auto even_count = CountIf(numbers, [](int n) { return n % 2 == 0; });
 * // even_count == 3
 * @endcode
 */
template <std::ranges::input_range Range, typename Predicate>
  requires std::predicate<Predicate, std::ranges::range_value_t<Range>>
auto CountIf(Range &&range, Predicate predicate) -> std::size_t {
  return static_cast<std::size_t>(std::ranges::count_if(range, predicate));
}

/**
 * @brief Transform a range into a vector using a transformation function
 *
 * @tparam Range Type of the input range. Must satisfy std::ranges::input_range.
 * @tparam Transform Type of the transformation function
 * @param range The input range to transform
 * @param transform The transformation function to apply to each element
 * @return A vector containing the transformed elements
 *
 * Applies the transformation function to each element in the input range and
 * collects the results in a new vector. The transformation function must be
 * callable with elements of the range.
 *
 * @code
 * std::vector<int> numbers{1, 2, 3, 4, 5};
 * auto squares = TransformToVector(numbers, [](int n) { return n * n; });
 * // squares == {1, 4, 9, 16, 25}
 * @endcode
 */
template <std::ranges::input_range Range, typename Transform>
auto TransformToVector(Range &&range, Transform transform)
    -> std::vector<std::invoke_result_t<Transform, std::ranges::range_value_t<Range>>> {
  using result_type = std::invoke_result_t<Transform, std::ranges::range_value_t<Range>>;
  std::vector<result_type> result;
  std::ranges::transform(range, std::back_inserter(result), transform);
  return result;
}

/**
 * @brief Find the minimum and maximum elements in a range
 *
 * @tparam Range Type of the input range. Must satisfy std::ranges::input_range.
 * @param range The input range to search for minimum and maximum elements
 * @return A pair containing the minimum and maximum elements (min, max)
 *
 * Finds both the minimum and maximum elements in the range in a single pass.
 *
 * @code
 * std::vector<int> numbers{3, 1, 4, 1, 5, 9, 2, 6};
 * auto [min_val, max_val] = FindMinMax(numbers);
 * // min_val == 1, max_val == 9
 * @endcode
 */
template <std::ranges::input_range Range>
[[nodiscard]] auto FindMinMax(Range &&range)
    -> std::pair<std::ranges::range_value_t<Range>, std::ranges::range_value_t<Range>> {
  auto [min_it, max_it] = std::ranges::minmax_element(range);
  return {*min_it, *max_it};
}

}  // namespace cpp_features::algorithms
