/**
 * @file exceptions_binding.cpp
 * @brief Python bindings for the exceptions module
 */

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "exceptions/custom_exception.hpp"
#include "exceptions/result.hpp"

namespace py = pybind11;

void bind_exceptions(py::module &m) {
  using namespace cpp_features::exceptions;

  // Bind ErrorSeverity enum
  py::enum_<ErrorSeverity>(m, "ErrorSeverity")
      .value("TRACE", ErrorSeverity::kTrace)
      .value("DEBUG", ErrorSeverity::kDebug)
      .value("INFO", ErrorSeverity::kInfo)
      .value("WARNING", ErrorSeverity::kWarning)
      .value("ERROR", ErrorSeverity::kError)
      .value("FATAL", ErrorSeverity::kFatal);

  m.def("severity_to_string", &SeverityToString, "Convert ErrorSeverity to string");

  // Bind BaseException
  py::exception<BaseException>(m, "BaseException")
      .def(py::init<std::string_view, ErrorSeverity>(), py::arg("message"),
           py::arg("severity") = ErrorSeverity::kError)
      .def("what", &BaseException::what)
      .def("get_severity", &BaseException::GetSeverity)
      .def("get_location", &BaseException::GetLocation, py::return_value_policy::reference_internal)
      .def("get_formatted_message", &BaseException::GetFormattedMessage)
      .def("__str__", [](const BaseException &e) { return std::string(e.what()); })
      .def("__repr__", [](const BaseException &e) {
        return std::format("<BaseException('{}') at {}>", e.what(), static_cast<const void *>(&e));
      });

  // Bind ValidationException
  py::exception<ValidationException>(m, "ValidationException", m.attr("BaseException"))
      .def(py::init<std::string_view, std::optional<std::string_view>>(), py::arg("message"),
           py::arg("field_name") = std::nullopt)
      .def("get_field_name", &ValidationException::GetFieldName)
      .def("__repr__", [](const ValidationException &e) {
        auto field = e.GetFieldName();
        return std::format("<ValidationException('{}', field='{}') at {}>", e.what(),
                           field.has_value() ? std::string(field.value()) : "None",
                           static_cast<const void *>(&e));
      });

  // Bind ResourceException
  py::exception<ResourceException>(m, "ResourceException", m.attr("BaseException"))
      .def(py::init<std::string_view, std::optional<std::string_view>>(), py::arg("message"),
           py::arg("resource_name") = std::nullopt)
      .def("get_resource_name", &ResourceException::GetResourceName)
      .def("__repr__", [](const ResourceException &e) {
        auto resource = e.GetResourceName();
        return std::format("<ResourceException('{}', resource='{}') at {}>", e.what(),
                           resource.has_value() ? std::string(resource.value()) : "None",
                           static_cast<const void *>(&e));
      });

  // Bind CalculationException
  py::exception<CalculationException>(m, "CalculationException", m.attr("BaseException"))
      .def(py::init<std::string_view, double>(), py::arg("message"), py::arg("input_value") = 0.0)
      .def("get_input_value", &CalculationException::GetInputValue)
      .def("__repr__", [](const CalculationException &e) {
        return std::format("<CalculationException('{}', value={}) at {}>", e.what(),
                           e.GetInputValue(), static_cast<const void *>(&e));
      });

  // Bind Result<int>
  py::class_<Result<int>>(m, "IntResult")
      .def(py::init<int>())
      .def(py::init<BaseException>())
      .def("has_value", &Result<int>::HasValue)
      .def("get_value", &Result<int>::GetValue, py::return_value_policy::reference_internal)
      .def("get_exception", &Result<int>::GetException, py::return_value_policy::reference_internal)
      .def("visit", &Result<int>::Visit<std::function<py::object(const auto &)>>)
      .def("map", &Result<int>::Map<std::function<int(const int &)>>)
      .def("then", &Result<int>::Then<std::function<Result<int>(const int &)>>)
      .def("__bool__", &Result<int>::HasValue)
      .def("__str__",
           [](const Result<int> &r) {
             return r.HasValue() ? std::format("Result({})", r.GetValue())
                                 : std::format("Result(Error: {})", r.GetException().GetMessage());
           })
      .def("__repr__", [](const Result<int> &r) {
        return std::format("<IntResult({}) at {}>",
                           r.HasValue() ? std::to_string(r.GetValue()) : "Error",
                           static_cast<const void *>(&r));
      });

  // Bind Result<double>
  py::class_<Result<double>>(m, "DoubleResult")
      .def(py::init<double>())
      .def(py::init<BaseException>())
      .def("has_value", &Result<double>::HasValue)
      .def("get_value", &Result<double>::GetValue, py::return_value_policy::reference_internal)
      .def("get_exception", &Result<double>::GetException,
           py::return_value_policy::reference_internal)
      .def("visit", &Result<double>::Visit<std::function<py::object(const auto &)>>)
      .def("map", &Result<double>::Map<std::function<double(const double &)>>)
      .def("then", &Result<double>::Then<std::function<Result<double>(const double &)>>)
      .def("__bool__", &Result<double>::HasValue)
      .def("__str__",
           [](const Result<double> &r) {
             return r.HasValue() ? std::format("Result({})", r.GetValue())
                                 : std::format("Result(Error: {})", r.GetException().GetMessage());
           })
      .def("__repr__", [](const Result<double> &r) {
        return std::format("<DoubleResult({}) at {}>",
                           r.HasValue() ? std::to_string(r.GetValue()) : "Error",
                           static_cast<const void *>(&r));
      });

  // Bind Result<std::string>
  py::class_<Result<std::string>>(m, "StringResult")
      .def(py::init<std::string>())
      .def(py::init<BaseException>())
      .def("has_value", &Result<std::string>::HasValue)
      .def("get_value", &Result<std::string>::GetValue, py::return_value_policy::reference_internal)
      .def("get_exception", &Result<std::string>::GetException,
           py::return_value_policy::reference_internal)
      .def("visit", &Result<std::string>::Visit<std::function<py::object(const auto &)>>)
      .def("map", &Result<std::string>::Map<std::function<std::string(const std::string &)>>)
      .def("then",
           &Result<std::string>::Then<std::function<Result<std::string>(const std::string &)>>)
      .def("__bool__", &Result<std::string>::HasValue)
      .def("__str__",
           [](const Result<std::string> &r) {
             return r.HasValue() ? std::format("Result('{}')", r.GetValue())
                                 : std::format("Result(Error: {})", r.GetException().GetMessage());
           })
      .def("__repr__", [](const Result<std::string> &r) {
        return std::format("<StringResult({}) at {}>",
                           r.HasValue() ? "'" + r.GetValue() + "'" : "Error",
                           static_cast<const void *>(&r));
      });
}
