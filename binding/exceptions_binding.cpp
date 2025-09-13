/**
 * @file exceptions_binding.cpp
 * @brief Python bindings for the exceptions module
 */

#include <concepts>
#include <exception>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include "exceptions/custom_exception.hpp"

namespace py = pybind11;
using namespace cpp_features::exceptions;

namespace {

auto MakePyOptionalString(const std::optional<std::string_view> &arg) -> py::object {
  if (arg) {
    return py::str{*arg};
  }
  return py::none{};
}

template <typename... Args>
  requires((std::derived_from<std::remove_cvref_t<Args>, py::object>) && ...)
auto TranslateException(const BaseException &e, const char *name, Args &&...args) {
  auto m = py::module_::import("demo.exceptions");
  auto cls = m.attr(name);
  auto inst = cls(py::str{e.what()}, std::forward<Args>(args)...);
  py::set_error(cls, inst);
}

// Custom translator to preserve custom fields when C++ exceptions cross into Python
// NOLINTNEXTLINE(performance-unnecessary-value-param)
auto TranslateExceptions(std::exception_ptr p) {
  if (!p) {
    return;
  }
  try {
    std::rethrow_exception(p);
  } catch (const ValidationException &e) {
    py::object field{MakePyOptionalString(e.GetFieldName())};
    TranslateException(e, "ValidationException", std::move(field));
  } catch (const ResourceException &e) {
    py::object resource{MakePyOptionalString(e.GetResourceName())};
    TranslateException(e, "ResourceException", std::move(resource));
  } catch (const CalculationException &e) {
    py::float_ input_value{e.GetInputValue()};
    TranslateException(e, "CalculationException", std::move(input_value));
  } catch (const BaseException &e) {
    auto m = py::module_::import("demo.exceptions");
    auto severity_cls = m.attr("ErrorSeverity");
    py::object severity = severity_cls(e.GetSeverity());
    TranslateException(e, "BaseException", std::move(severity));
  }
}

auto TestThrowValidationException() {
  throw ValidationException{"Test validation exception", "test_field"};
}

auto TestThrowResourceException() {
  throw ResourceException{"Test resource exception", "test_resource"};
}

auto TestThrowCalculationException() {
  throw CalculationException{"Test calculation exception", 1.0};
}

auto TestThrowBaseException() {
  throw BaseException{"Test base exception", ErrorSeverity::kWarning};
}

auto TestThrowUnknownException() { throw std::runtime_error{"Test unknown exception"}; }

}  // namespace

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
  py::register_exception_translator(&TranslateExceptions);

  // Bind test functions
  m.def("test_throw_validation_exception", &TestThrowValidationException);
  m.def("test_throw_resource_exception", &TestThrowResourceException);
  m.def("test_throw_calculation_exception", &TestThrowCalculationException);
  m.def("test_throw_base_exception", &TestThrowBaseException);
  m.def("test_throw_unknown_exception", &TestThrowUnknownException);
}
