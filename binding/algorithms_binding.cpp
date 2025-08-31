/**
 * @file algorithms_binding.cpp
 * @brief Python bindings for the algorithms module
 */

#include <cstddef>
#include <functional>
#include <ranges>
#include <string>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

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

template <std::ranges::input_range Range>
auto TransformToVectorWrapper(
    const Range &range,
    const std::function<py::object(std::ranges::range_value_t<Range>)> &transform) {
  return TransformToVector(range, transform);
}

template <std::ranges::input_range Range>
auto FindMinMaxWrapper(const Range &range) {
  return FindMinMax(range);
}

// Template function to register algorithms bindings for a specific type
template <typename T>
void BindAlgorithmsFor(py::module &m) {
  m.def("sort", &SortContainer<Container<T>>);
  m.def("count_if", &CountIfWrapper<Container<T>>);
  m.def("transform_to_list", &TransformToVectorWrapper<Container<T>>);
  m.def("find_min_max", &FindMinMaxWrapper<Container<T>>);
}

}  // namespace

void BindAlgorithms(py::module &m) {
  BindAlgorithmsFor<int>(m);
  BindAlgorithmsFor<double>(m);
  BindAlgorithmsFor<std::string>(m);
}
