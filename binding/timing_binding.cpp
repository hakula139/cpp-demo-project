/**
 * @file timing_binding.cpp
 * @brief Python bindings for the timing module
 */

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "timing/timer.hpp"

namespace py = pybind11;

void bind_timing(py::module &m) {
  using namespace cpp_features::timing;

  // Bind utility functions
  m.def("to_human_readable", &ToHumanReadable, "Convert nanoseconds to human-readable string");

  m.def(
      "time_function", [](std::function<void()> func) { return TimeFunction(func); },
      "Time a function execution and return elapsed nanoseconds");

  // Bind Timer class
  py::class_<Timer>(m, "Timer")
      .def(py::init<>())
      .def("start", &Timer::Start)
      .def("stop", &Timer::Stop)
      .def("reset", &Timer::Reset)
      .def("get_elapsed_ns",
           [](const Timer &self) { return self.GetElapsed<std::chrono::nanoseconds>(); })
      .def("get_elapsed_us",
           [](const Timer &self) { return self.GetElapsed<std::chrono::microseconds>(); })
      .def("get_elapsed_ms",
           [](const Timer &self) { return self.GetElapsed<std::chrono::milliseconds>(); })
      .def("get_elapsed_s",
           [](const Timer &self) { return self.GetElapsed<std::chrono::seconds>(); })
      .def("get_elapsed_string", &Timer::GetElapsedString)
      .def("__str__", &Timer::GetElapsedString)
      .def("__repr__", [](const Timer &t) {
        return std::format("<Timer(elapsed='{}') at {}>", t.GetElapsedString(),
                           static_cast<const void *>(&t));
      });

  // Bind ScopedTimer class with context manager support
  py::class_<ScopedTimer>(m, "ScopedTimer")
      .def(py::init<std::string_view>())
      .def(py::init<std::string_view, std::function<void(std::int64_t)>>())
      .def("__repr__",
           [](const ScopedTimer &st) {
             return std::format("<ScopedTimer at {}>", static_cast<const void *>(&st));
           })
      .def(
          "__enter__", [](ScopedTimer &self) { return &self; }, py::return_value_policy::reference)
      .def("__exit__", [](ScopedTimer & /*self*/, py::object, py::object, py::object) {
        // Destructor will be called automatically
        return false;
      });
}
