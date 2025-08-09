/**
 * @file containers_binding.cpp
 * @brief Python bindings for the containers module
 */

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "containers/container.hpp"

namespace py = pybind11;

void bind_containers(py::module &m) {
  using namespace cpp_features::containers;

  // Bind ContainerError enum
  py::enum_<ContainerError>(m, "ContainerError")
      .value("EMPTY", ContainerError::kEmpty)
      .value("OUT_OF_BOUNDS", ContainerError::kOutOfBounds)
      .value("INVALID_OPERATION", ContainerError::kInvalidOperation);

  // Bind Container<int>
  py::class_<Container<int>>(m, "IntContainer")
      .def(py::init<>())
      .def(py::init<Container<int>::size_type>())
      .def(py::init<std::initializer_list<int>>())
      .def(py::init<std::vector<int>>())
      .def("add", py::overload_cast<const int &>(&Container<int>::Add))
      .def("add", py::overload_cast<int &&>(&Container<int>::Add))
      .def("emplace", &Container<int>::Emplace<int>)
      .def("remove", &Container<int>::Remove)
      .def("get_size", &Container<int>::GetSize)
      .def("size", &Container<int>::size)
      .def("is_empty", &Container<int>::IsEmpty)
      .def("at",
           [](const Container<int> &self, Container<int>::size_type index) {
             auto result = self.At(index);
             if (result) {
               return result->get();
             } else {
               throw py::index_error("Index out of bounds");
             }
           })
      .def("get_view", &Container<int>::GetView)
      .def("get_filtered_view", &Container<int>::GetFilteredView<std::function<bool(int)>>)
      .def("get_transformed_view", &Container<int>::GetTransformedView<std::function<int(int)>>)
      .def("__len__", &Container<int>::GetSize)
      .def("__bool__", [](const Container<int> &self) { return !self.IsEmpty(); })
      .def("__getitem__",
           [](const Container<int> &self, Container<int>::size_type index) {
             auto result = self.At(index);
             if (result) {
               return result->get();
             } else {
               throw py::index_error("Index out of bounds");
             }
           })
      .def(
          "__iter__",
          [](const Container<int> &self) { return py::make_iterator(self.begin(), self.end()); },
          py::keep_alive<0, 1>())
      .def("__str__", [](const Container<int> &self) { return std::format("{}", self); })
      .def("__repr__", [](const Container<int> &self) {
        return std::format("<IntContainer(size={}) at {}>", self.GetSize(),
                           static_cast<const void *>(&self));
      });

  // Bind Container<double>
  py::class_<Container<double>>(m, "DoubleContainer")
      .def(py::init<>())
      .def(py::init<Container<double>::size_type>())
      .def(py::init<std::initializer_list<double>>())
      .def(py::init<std::vector<double>>())
      .def("add", py::overload_cast<const double &>(&Container<double>::Add))
      .def("add", py::overload_cast<double &&>(&Container<double>::Add))
      .def("emplace", &Container<double>::Emplace<double>)
      .def("remove", &Container<double>::Remove)
      .def("get_size", &Container<double>::GetSize)
      .def("size", &Container<double>::size)
      .def("is_empty", &Container<double>::IsEmpty)
      .def("at",
           [](const Container<double> &self, Container<double>::size_type index) {
             auto result = self.At(index);
             if (result) {
               return result->get();
             } else {
               throw py::index_error("Index out of bounds");
             }
           })
      .def("get_view", &Container<double>::GetView)
      .def("get_filtered_view", &Container<double>::GetFilteredView<std::function<bool(double)>>)
      .def("get_transformed_view",
           &Container<double>::GetTransformedView<std::function<double(double)>>)
      .def("__len__", &Container<double>::GetSize)
      .def("__bool__", [](const Container<double> &self) { return !self.IsEmpty(); })
      .def("__getitem__",
           [](const Container<double> &self, Container<double>::size_type index) {
             auto result = self.At(index);
             if (result) {
               return result->get();
             } else {
               throw py::index_error("Index out of bounds");
             }
           })
      .def(
          "__iter__",
          [](const Container<double> &self) { return py::make_iterator(self.begin(), self.end()); },
          py::keep_alive<0, 1>())
      .def("__str__", [](const Container<double> &self) { return std::format("{}", self); })
      .def("__repr__", [](const Container<double> &self) {
        return std::format("<DoubleContainer(size={}) at {}>", self.GetSize(),
                           static_cast<const void *>(&self));
      });

  // Bind Container<std::string>
  py::class_<Container<std::string>>(m, "StringContainer")
      .def(py::init<>())
      .def(py::init<Container<std::string>::size_type>())
      .def(py::init<std::initializer_list<std::string>>())
      .def(py::init<std::vector<std::string>>())
      .def("add", py::overload_cast<const std::string &>(&Container<std::string>::Add))
      .def("add", py::overload_cast<std::string &&>(&Container<std::string>::Add))
      .def("emplace", &Container<std::string>::Emplace<std::string>)
      .def("remove", &Container<std::string>::Remove)
      .def("get_size", &Container<std::string>::GetSize)
      .def("size", &Container<std::string>::size)
      .def("is_empty", &Container<std::string>::IsEmpty)
      .def("at",
           [](const Container<std::string> &self, Container<std::string>::size_type index) {
             auto result = self.At(index);
             if (result) {
               return result->get();
             } else {
               throw py::index_error("Index out of bounds");
             }
           })
      .def("get_view", &Container<std::string>::GetView)
      .def("get_filtered_view",
           &Container<std::string>::GetFilteredView<std::function<bool(const std::string &)>>)
      .def("get_transformed_view", &Container<std::string>::GetTransformedView<
                                       std::function<std::string(const std::string &)>>)
      .def("__len__", &Container<std::string>::GetSize)
      .def("__bool__", [](const Container<std::string> &self) { return !self.IsEmpty(); })
      .def("__getitem__",
           [](const Container<std::string> &self, Container<std::string>::size_type index) {
             auto result = self.At(index);
             if (result) {
               return result->get();
             } else {
               throw py::index_error("Index out of bounds");
             }
           })
      .def(
          "__iter__",
          [](const Container<std::string> &self) {
            return py::make_iterator(self.begin(), self.end());
          },
          py::keep_alive<0, 1>())
      .def("__str__", [](const Container<std::string> &self) { return std::format("{}", self); })
      .def("__repr__", [](const Container<std::string> &self) {
        return std::format("<StringContainer(size={}) at {}>", self.GetSize(),
                           static_cast<const void *>(&self));
      });
}
