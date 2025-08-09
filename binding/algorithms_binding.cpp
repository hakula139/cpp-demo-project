/**
 * @file algorithms_binding.cpp
 * @brief Python bindings for the algorithms module
 */

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "algorithms/stl.hpp"
#include "containers/container.hpp"

namespace py = pybind11;

void bind_algorithms(py::module &m) {
  using namespace cpp_features::algorithms;
  using namespace cpp_features::containers;

  // Bind sort functions
  m.def(
      "sort_container", [](Container<int> &container) { SortContainer(container); },
      "Sort an integer container");

  m.def(
      "sort_container", [](Container<double> &container) { SortContainer(container); },
      "Sort a double container");

  m.def(
      "sort_container", [](Container<std::string> &container) { SortContainer(container); },
      "Sort a string container");

  m.def(
      "sort_container", [](std::vector<int> &container) { SortContainer(container); },
      "Sort an integer vector");

  m.def(
      "sort_container", [](std::vector<double> &container) { SortContainer(container); },
      "Sort a double vector");

  m.def(
      "sort_container", [](std::vector<std::string> &container) { SortContainer(container); },
      "Sort a string vector");

  // Bind count_if functions
  m.def(
      "count_if",
      [](const std::vector<int> &range, std::function<bool(int)> predicate) {
        return CountIf(range, predicate);
      },
      "Count elements matching predicate in integer vector");

  m.def(
      "count_if",
      [](const std::vector<double> &range, std::function<bool(double)> predicate) {
        return CountIf(range, predicate);
      },
      "Count elements matching predicate in double vector");

  m.def(
      "count_if",
      [](const std::vector<std::string> &range,
         std::function<bool(const std::string &)> predicate) { return CountIf(range, predicate); },
      "Count elements matching predicate in string vector");

  m.def(
      "count_if",
      [](const Container<int> &range, std::function<bool(int)> predicate) {
        return CountIf(range, predicate);
      },
      "Count elements matching predicate in integer container");

  m.def(
      "count_if",
      [](const Container<double> &range, std::function<bool(double)> predicate) {
        return CountIf(range, predicate);
      },
      "Count elements matching predicate in double container");

  // Bind transform functions
  m.def(
      "transform_to_vector",
      [](const std::vector<int> &range, std::function<int(int)> transform) {
        return TransformToVector(range, transform);
      },
      "Transform integer vector to new vector");

  m.def(
      "transform_to_vector",
      [](const std::vector<double> &range, std::function<double(double)> transform) {
        return TransformToVector(range, transform);
      },
      "Transform double vector to new vector");

  m.def(
      "transform_to_vector",
      [](const std::vector<int> &range, std::function<double(int)> transform) {
        return TransformToVector(range, transform);
      },
      "Transform integer vector to double vector");

  m.def(
      "transform_to_vector",
      [](const Container<int> &range, std::function<int(int)> transform) {
        return TransformToVector(range, transform);
      },
      "Transform integer container to vector");

  m.def(
      "transform_to_vector",
      [](const Container<double> &range, std::function<double(double)> transform) {
        return TransformToVector(range, transform);
      },
      "Transform double container to vector");

  // Bind find_min_max functions
  m.def(
      "find_min_max", [](const std::vector<int> &range) { return FindMinMax(range); },
      "Find minimum and maximum in integer vector");

  m.def(
      "find_min_max", [](const std::vector<double> &range) { return FindMinMax(range); },
      "Find minimum and maximum in double vector");

  m.def(
      "find_min_max", [](const Container<int> &range) { return FindMinMax(range); },
      "Find minimum and maximum in integer container");

  m.def(
      "find_min_max", [](const Container<double> &range) { return FindMinMax(range); },
      "Find minimum and maximum in double container");
}
