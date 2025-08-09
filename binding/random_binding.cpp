/**
 * @file random_binding.cpp
 * @brief Python bindings for the random module
 */

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "random/random_gen.hpp"

namespace py = pybind11;

void bind_random(py::module &m) {
  using namespace cpp_features::random;

  // Bind RandomGenerator class
  py::class_<RandomGenerator>(m, "RandomGenerator")
      .def(py::init<>())
      .def(py::init<std::uint32_t>())
      .def("generate_int", &RandomGenerator::GenerateInt<int>)
      .def("generate_long", &RandomGenerator::GenerateInt<long>)
      .def("generate_real", &RandomGenerator::GenerateReal<double>)
      .def("generate_float", &RandomGenerator::GenerateReal<float>)
      .def("generate_int_vector", &RandomGenerator::GenerateIntVector<int>)
      .def("generate_long_vector", &RandomGenerator::GenerateIntVector<long>)
      .def("generate_real_vector", &RandomGenerator::GenerateRealVector<double>)
      .def("generate_float_vector", &RandomGenerator::GenerateRealVector<float>)
      .def("generate_bool", &RandomGenerator::GenerateBool, py::arg("probability") = 0.5)
      .def("generate_normal", &RandomGenerator::GenerateNormal<double>)
      .def("generate_normal_float", &RandomGenerator::GenerateNormal<float>)
      .def("seed", &RandomGenerator::Seed)
      .def("seed_with_time", &RandomGenerator::SeedWithTime)
      .def("__repr__", [](const RandomGenerator &rg) {
        return std::format("<RandomGenerator at {}>", static_cast<const void *>(&rg));
      });

  // Bind utility functions
  m.def(
      "shuffle_container", [](std::vector<int> &container) { ShuffleContainer(container); },
      "Shuffle an integer vector");

  m.def(
      "shuffle_container", [](std::vector<double> &container) { ShuffleContainer(container); },
      "Shuffle a double vector");

  m.def(
      "shuffle_container", [](std::vector<std::string> &container) { ShuffleContainer(container); },
      "Shuffle a string vector");

  m.def(
      "sample_from_range",
      [](const std::vector<int> &range, std::size_t count) {
        return SampleFromRange(range, count);
      },
      "Sample from integer vector");

  m.def(
      "sample_from_range",
      [](const std::vector<double> &range, std::size_t count) {
        return SampleFromRange(range, count);
      },
      "Sample from double vector");

  m.def(
      "sample_from_range",
      [](const std::vector<std::string> &range, std::size_t count) {
        return SampleFromRange(range, count);
      },
      "Sample from string vector");

  m.def(
      "sample_from_range",
      [](const std::string &range, std::size_t count) { return SampleFromRange(range, count); },
      "Sample from string");
}
