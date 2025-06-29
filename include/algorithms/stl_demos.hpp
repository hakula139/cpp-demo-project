#pragma once

#include <algorithm>
#include <concepts>
#include <ranges>
#include <vector>

namespace cpp_features::algorithms {

template <typename Container>
concept SortableContainer = std::ranges::random_access_range<Container> &&
                            std::sortable<std::ranges::iterator_t<Container>>;

template <typename T>
concept Comparable = std::totally_ordered<T>;

void DemonstrateStlAlgorithms();

void DemonstrateRangesAndViews();

template <SortableContainer Container>
void SortContainer(Container& container) {
  std::ranges::sort(container);
}

template <std::ranges::input_range Range, typename Predicate>
  requires std::predicate<Predicate, std::ranges::range_value_t<Range>>
auto CountIf(Range&& range, Predicate predicate) -> std::size_t {
  return std::ranges::count_if(range, predicate);
}

template <std::ranges::input_range Range, typename Transform>
auto TransformToVector(Range&& range, Transform transform)
    -> std::vector<std::invoke_result_t<Transform, std::ranges::range_value_t<Range>>> {
  using result_type = std::invoke_result_t<Transform, std::ranges::range_value_t<Range>>;
  std::vector<result_type> result;
  std::ranges::transform(range, std::back_inserter(result), transform);
  return result;
}

template <Comparable T>
[[nodiscard]] auto FindMinMax(const std::vector<T>& data) -> std::pair<T, T> {
  auto [min_it, max_it] = std::ranges::minmax_element(data);
  return {*min_it, *max_it};
}

void DemonstrateParallelAlgorithms();

void DemonstrateCustomAlgorithms();

}  // namespace cpp_features::algorithms
