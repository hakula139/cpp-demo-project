/**
 * @file timing_binding.cpp
 * @brief Python bindings for the timing module
 */

#include <chrono>
#include <cstddef>
#include <functional>
#include <string>

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

#include "timing/benchmark.hpp"
#include "timing/timer.hpp"

namespace py = pybind11;
using namespace cpp_features::timing;

namespace {

template <typename DurationType = std::chrono::milliseconds>
auto GetElapsedWrapper(const Timer &timer) {
  return timer.GetElapsed<DurationType>();
}

auto BenchmarkWrapper(const std::string &name, const std::function<void()> &func,
                      std::size_t iterations = 1000) {
  return BenchmarkRunner::Benchmark(name, func, iterations);
}

auto TimeFunctionWrapper(const std::function<void()> &func) { return TimeFunction(func); }

}  // namespace

void BindTiming(py::module &m) {
  // Bind Timer class
  py::class_<Timer>(m, "Timer")
      .def(py::init<>())
      .def("start", &Timer::Start)
      .def("stop", &Timer::Stop)
      .def("reset", &Timer::Reset)
      .def("get_elapsed_ns", &GetElapsedWrapper<std::chrono::nanoseconds>)
      .def("get_elapsed_us", &GetElapsedWrapper<std::chrono::microseconds>)
      .def("get_elapsed_ms", &GetElapsedWrapper<std::chrono::milliseconds>)
      .def("get_elapsed_s", &GetElapsedWrapper<std::chrono::seconds>)
      .def("get_elapsed_str", &Timer::GetElapsedString);

  // Bind BenchmarkResult struct
  py::class_<BenchmarkRunner::BenchmarkResult>(m, "BenchmarkResult")
      .def_readwrite("name", &BenchmarkRunner::BenchmarkResult::name)
      .def_readwrite("iterations", &BenchmarkRunner::BenchmarkResult::iterations)
      .def_readwrite("total_ns", &BenchmarkRunner::BenchmarkResult::total_ns)
      .def_readwrite("avg_ns", &BenchmarkRunner::BenchmarkResult::avg_ns)
      .def_readwrite("min_ns", &BenchmarkRunner::BenchmarkResult::min_ns)
      .def_readwrite("max_ns", &BenchmarkRunner::BenchmarkResult::max_ns);

  // Bind BenchmarkRunner class
  py::class_<BenchmarkRunner>(m, "BenchmarkRunner")
      .def_static("benchmark", &BenchmarkWrapper)
      .def_static("print_result", &BenchmarkRunner::PrintResult);

  // Bind utility functions
  m.def("to_human_readable", &ToHumanReadable);
  m.def("time_function", &TimeFunctionWrapper);
}
