/**
 * @file timing_binding.cpp
 * @brief Python bindings for the timing module
 */

#include <chrono>
#include <cstdint>
#include <format>
#include <functional>

#include <pybind11/pybind11.h>

#include "timing/timer.hpp"

namespace py = pybind11;
using namespace cpp_features::timing;

namespace {

template <typename DurationType = std::chrono::milliseconds>
auto GetElapsedWrapper(const Timer &timer) {
  return timer.GetElapsed<DurationType>();
}

auto TimeFunctionWrapper(const std::function<void()> &func) -> std::int64_t {
  return TimeFunction(func);
}

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

  // Bind utility functions
  m.def("to_human_readable", &ToHumanReadable);
  m.def("time_function", &TimeFunctionWrapper);
}
