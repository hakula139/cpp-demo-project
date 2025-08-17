/**
 * @file containers_binding.cpp
 * @brief Python bindings for the containers module
 */

#include <format>
#include <functional>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

#include <pybind11/attr.h>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>

#include "concepts/concepts.hpp"
#include "concepts/utility_concepts.hpp"
#include "containers/container.hpp"

namespace py = pybind11;
using namespace cpp_features::containers;
using cpp_features::concepts::CopyableType;

namespace {

template <CopyableType T>
auto GetItem(const Container<T> &self, typename Container<T>::size_type index) -> T {
  if (auto result = self.At(index); result) {
    return result->get();
  }
  throw py::index_error("Index out of bounds");
}

template <CopyableType T>
auto GetIter(const Container<T> &self) {
  return py::make_iterator(self.begin(), self.end());
}

template <CopyableType T>
auto GetRepr(const Container<T> &self, std::string_view type_name) {
  return std::format("<{}Container(size={}) at {}>", type_name, self.GetSize(),
                     static_cast<const void *>(&self));
}

}  // namespace

void BindContainers(py::module &m) {
  using IntContainer = Container<int>;
  using FloatContainer = Container<double>;
  using StringContainer = Container<std::string>;

  // Bind ContainerError enum
  py::enum_<ContainerError>(m, "ContainerError")
      .value("EMPTY", ContainerError::kEmpty)
      .value("OUT_OF_BOUNDS", ContainerError::kOutOfBounds)
      .value("INVALID_OPERATION", ContainerError::kInvalidOperation);

  // Bind Container<int>
  py::class_<IntContainer>(m, "IntContainer")
      .def(py::init<>())
      .def(py::init<IntContainer::size_type>())
      .def(py::init<std::initializer_list<int>>())
      .def(py::init<std::vector<int>>())
      .def("add", py::overload_cast<const int &>(&IntContainer::Add))
      .def("remove", &IntContainer::Remove)
      .def("size", &IntContainer::GetSize)
      .def("empty", &IntContainer::IsEmpty)
      .def("at", &GetItem<int>)
      .def("view", &IntContainer::GetView)
      .def("filter", &IntContainer::GetFilteredView<std::function<bool(int)>>)
      .def("transform", &IntContainer::GetTransformedView<std::function<int(int)>>)
      .def("__len__", &IntContainer::GetSize)
      .def("__bool__", [](const IntContainer &self) { return !self.IsEmpty(); })
      .def("__getitem__", &GetItem<int>)
      .def("__iter__", &GetIter<int>, py::keep_alive<0, 1>())
      .def("__str__", [](const IntContainer &self) { return std::format("{}", self); })
      .def("__repr__", [](const IntContainer &self) { return GetRepr(self, "Int"); });

  // Bind Container<double>
  py::class_<FloatContainer>(m, "FloatContainer")
      .def(py::init<>())
      .def(py::init<FloatContainer::size_type>())
      .def(py::init<std::initializer_list<double>>())
      .def(py::init<std::vector<double>>())
      .def("add", py::overload_cast<const double &>(&FloatContainer::Add))
      .def("remove", &FloatContainer::Remove)
      .def("size", &FloatContainer::GetSize)
      .def("empty", &FloatContainer::IsEmpty)
      .def("at", &GetItem<double>)
      .def("view", &FloatContainer::GetView)
      .def("filter", &FloatContainer::GetFilteredView<std::function<bool(double)>>)
      .def("transform", &FloatContainer::GetTransformedView<std::function<double(double)>>)
      .def("__len__", &FloatContainer::GetSize)
      .def("__bool__", [](const FloatContainer &self) { return !self.IsEmpty(); })
      .def("__getitem__", &GetItem<double>)
      .def("__iter__", &GetIter<double>, py::keep_alive<0, 1>())
      .def("__str__", [](const FloatContainer &self) { return std::format("{}", self); })
      .def("__repr__", [](const FloatContainer &self) { return GetRepr(self, "Float"); });

  // Bind Container<std::string>
  py::class_<StringContainer>(m, "StringContainer")
      .def(py::init<>())
      .def(py::init<StringContainer::size_type>())
      .def(py::init<std::initializer_list<std::string>>())
      .def(py::init<std::vector<std::string>>())
      .def("add", py::overload_cast<const std::string &>(&StringContainer::Add))
      .def("remove", &StringContainer::Remove)
      .def("size", &StringContainer::GetSize)
      .def("empty", &StringContainer::IsEmpty)
      .def("at", &GetItem<std::string>)
      .def("view", &StringContainer::GetView)
      .def("filter", &StringContainer::GetFilteredView<std::function<bool(const std::string &)>>)
      .def("transform",
           &StringContainer::GetTransformedView<std::function<std::string(const std::string &)>>)
      .def("__len__", &StringContainer::GetSize)
      .def("__bool__", [](const StringContainer &self) { return !self.IsEmpty(); })
      .def("__getitem__", &GetItem<std::string>)
      .def("__iter__", &GetIter<std::string>, py::keep_alive<0, 1>())
      .def("__str__", [](const StringContainer &self) { return std::format("{}", self); })
      .def("__repr__", [](const StringContainer &self) { return GetRepr(self, "String"); });
}
