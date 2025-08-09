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
      .def("generate_int", &RandomGenerator::GenerateInt<std::int64_t>)
      .def("generate_real", &RandomGenerator::GenerateReal<double>)
      .def("generate_int_list", &RandomGenerator::GenerateIntVector<int>)
      .def("generate_real_list", &RandomGenerator::GenerateRealVector<double>)
      .def("generate_bool", &RandomGenerator::GenerateBool)
      .def("generate_normal", &RandomGenerator::GenerateNormal<double>)
      .def("generate_normal_float", &RandomGenerator::GenerateNormal<float>)
      .def("seed", &RandomGenerator::Seed)
      .def("seed_with_time", &RandomGenerator::SeedWithTime)
      .def("__repr__", [](const RandomGenerator &self) {
        return std::format("<RandomGenerator at {}>", static_cast<const void *>(&self));
      });

  // Bind utility functions
  m.def("shuffle_container", &ShuffleContainer<Container<int>>);
  m.def("shuffle_container", &ShuffleContainer<Container<double>>);
  m.def("shuffle_container", &ShuffleContainer<Container<std::string>>);
  m.def("shuffle_container", &ShuffleContainer<std::vector<int>>);
  m.def("shuffle_container", &ShuffleContainer<std::vector<double>>);
  m.def("shuffle_container", &ShuffleContainer<std::vector<std::string>>);
  m.def("shuffle_container", &ShuffleContainer<std::string>);

  m.def("sample_from_range", &SampleFromRangeWrapper<Container<int>>);
  m.def("sample_from_range", &SampleFromRangeWrapper<Container<double>>);
  m.def("sample_from_range", &SampleFromRangeWrapper<Container<std::string>>);
  m.def("sample_from_range", &SampleFromRangeWrapper<std::vector<int>>);
  m.def("sample_from_range", &SampleFromRangeWrapper<std::vector<double>>);
  m.def("sample_from_range", &SampleFromRangeWrapper<std::vector<std::string>>);
  m.def("sample_from_range", &SampleFromRangeWrapper<std::string>);
}
