/**
 * @file random_binding.cpp
 * @brief Python bindings for the random module
 */

#include <cstddef>
#include <cstdint>
#include <format>
#include <ranges>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "containers/container.hpp"
#include "random/random_gen.hpp"

namespace py = pybind11;
using namespace cpp_features::random;
using cpp_features::containers::Container;

namespace {

template <std::ranges::input_range Range>
auto SampleFromRangeWrapper(const Range &range, std::size_t count) {
  return SampleFromRange(range, count);
}

}  // namespace

void BindRandom(py::module &m) {
  // Bind RandomGenerator class
  py::class_<RandomGenerator>(m, "RandomGenerator")
      .def(py::init<>())
      .def(py::init<std::uint32_t>())
      .def("rand_int", &RandomGenerator::GenerateInt<std::int64_t>)
      .def("rand_float", &RandomGenerator::GenerateReal<double>)
      .def("rand_ints", &RandomGenerator::GenerateIntVector<int>)
      .def("rand_floats", &RandomGenerator::GenerateRealVector<double>)
      .def("rand_bool", &RandomGenerator::GenerateBool)
      .def("normal", &RandomGenerator::GenerateNormal<double>)
      .def("seed", &RandomGenerator::Seed)
      .def("seed_with_time", &RandomGenerator::SeedWithTime)
      .def("__repr__", [](const RandomGenerator &self) {
        return std::format("<RandomGenerator at {}>", static_cast<const void *>(&self));
      });

  // Bind utility functions
  m.def("shuffle", &ShuffleContainer<Container<int>>);
  m.def("shuffle", &ShuffleContainer<Container<double>>);
  m.def("shuffle", &ShuffleContainer<Container<std::string>>);
  m.def("shuffle", &ShuffleContainer<std::vector<int>>);
  m.def("shuffle", &ShuffleContainer<std::vector<double>>);
  m.def("shuffle", &ShuffleContainer<std::vector<std::string>>);
  m.def("shuffle", &ShuffleContainer<std::string>);

  m.def("sample", &SampleFromRangeWrapper<Container<int>>);
  m.def("sample", &SampleFromRangeWrapper<Container<double>>);
  m.def("sample", &SampleFromRangeWrapper<Container<std::string>>);
  m.def("sample", &SampleFromRangeWrapper<std::vector<int>>);
  m.def("sample", &SampleFromRangeWrapper<std::vector<double>>);
  m.def("sample", &SampleFromRangeWrapper<std::vector<std::string>>);
  m.def("sample", &SampleFromRangeWrapper<std::string>);
}
