/**
 * @file exceptions_binding.cpp
 * @brief Python bindings for the exceptions module
 */

#include <pybind11/pybind11.h>

#include "exceptions/custom_exception.hpp"

namespace py = pybind11;
using namespace cpp_features::exceptions;

void BindExceptions(py::module &m) {
  // Bind ErrorSeverity enum
  py::enum_<ErrorSeverity>(m, "ErrorSeverity")
      .value("TRACE", ErrorSeverity::kTrace)
      .value("DEBUG", ErrorSeverity::kDebug)
      .value("INFO", ErrorSeverity::kInfo)
      .value("WARNING", ErrorSeverity::kWarning)
      .value("ERROR", ErrorSeverity::kError)
      .value("FATAL", ErrorSeverity::kFatal);

  m.def("severity_to_string", &SeverityToString);

  // Register exceptions so C++ throws translate to Python exceptions
  auto py_base_exception = py::register_exception<BaseException>(m, "BaseException");
  py::register_exception<ValidationException>(m, "ValidationException", py_base_exception.ptr());
  py::register_exception<ResourceException>(m, "ResourceException", py_base_exception.ptr());
  py::register_exception<CalculationException>(m, "CalculationException", py_base_exception.ptr());
}
