#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>

namespace cpp_features::concepts {

template <typename Container>
concept IterableContainer = requires(Container c) {
  { c.begin() } -> std::input_or_output_iterator;
  { c.end() } -> std::input_or_output_iterator;
  { c.size() } -> std::convertible_to<std::size_t>;
};

template <typename Container>
concept RangeContainer = std::ranges::range<Container>;

}  // namespace cpp_features::concepts
