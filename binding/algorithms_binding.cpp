/**
 * @file algorithms_binding.cpp
 * @brief Python bindings for the algorithms module
 */

#include <cstddef>
#include <functional>
#include <ranges>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>

#include "algorithms/stl.hpp"
#include "containers/container.hpp"

namespace py = pybind11;
using namespace cpp_features::algorithms;
using cpp_features::containers::Container;

namespace {

template <std::ranges::input_range Range>
auto CountIfWrapper(const Range &range,
                    const std::function<bool(std::ranges::range_value_t<Range>)> &predicate) {
  return CountIf(range, predicate);
}

template <std::ranges::input_range Range, typename U = std::ranges::range_value_t<Range>>
auto TransformToVectorWrapper(
    const Range &range, const std::function<U(std::ranges::range_value_t<Range>)> &transform) {
  return TransformToVector(range, transform);
}

template <std::ranges::input_range Range>
auto FindMinMaxWrapper(const Range &range) {
  return FindMinMax(range);
}

}  // namespace

void BindAlgorithms(py::module &m) {
  // Bind sort functions
  m.def("sort", &SortContainer<Container<int>>);
  m.def("sort", &SortContainer<Container<double>>);
  m.def("sort", &SortContainer<Container<std::string>>);
  m.def("sort", &SortContainer<std::vector<int>>);
  m.def("sort", &SortContainer<std::vector<double>>);
  m.def("sort", &SortContainer<std::vector<std::string>>);

  // Bind count_if functions
  m.def("count_if", &CountIfWrapper<Container<int>>);
  m.def("count_if", &CountIfWrapper<Container<double>>);
  m.def("count_if", &CountIfWrapper<Container<std::string>>);
  m.def("count_if", &CountIfWrapper<std::vector<int>>);
  m.def("count_if", &CountIfWrapper<std::vector<double>>);
  m.def("count_if", &CountIfWrapper<std::vector<std::string>>);

  // Bind transform functions
  m.def("transform_to_list", &TransformToVectorWrapper<Container<int>, int>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<int>, double>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<int>, std::string>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<double>, int>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<double>, double>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<double>, std::string>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<std::string>, int>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<std::string>, double>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<std::string>, std::string>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<int>, int>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<int>, double>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<int>, std::string>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<double>, int>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<double>, double>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<double>, std::string>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<std::string>, int>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<std::string>, double>);
  m.def("transform_to_list", &TransformToVectorWrapper<std::vector<std::string>, std::string>);

  // Bind find_min_max functions
  m.def("find_min_max", &FindMinMaxWrapper<Container<int>>);
  m.def("find_min_max", &FindMinMaxWrapper<Container<double>>);
  m.def("find_min_max", &FindMinMaxWrapper<Container<std::string>>);
  m.def("find_min_max", &FindMinMaxWrapper<std::vector<int>>);
  m.def("find_min_max", &FindMinMaxWrapper<std::vector<double>>);
  m.def("find_min_max", &FindMinMaxWrapper<std::vector<std::string>>);
}
