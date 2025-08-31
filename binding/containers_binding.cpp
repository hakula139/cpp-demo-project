/**
 * @file containers_binding.cpp
 * @brief Python bindings for the containers module
 */

#include <format>
#include <functional>
#include <initializer_list>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <pybind11/attr.h>
#include <pybind11/detail/common.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include "concepts/concepts.hpp"
#include "concepts/utility_concepts.hpp"
#include "containers/container.hpp"

namespace py = pybind11;
using namespace cpp_features::containers;
using cpp_features::concepts::CopyableType;
using cpp_features::concepts::TransformFunction;

namespace {

template <CopyableType T>
auto GetItem(const Container<T> &self, typename Container<T>::size_type index) -> T {
  if (auto result = self.At(index); result) {
    return result->get();
  }
  throw py::index_error("Index out of bounds");
}

template <typename T>
auto GetFilteredWrapper(const Container<T> &self, const std::function<bool(T)> &filter) {
  auto filtered_view = self.template GetFilteredView<std::function<bool(T)>>(filter);
  return std::ranges::to<std::vector>(std::move(filtered_view));
}

template <typename Input>
auto GetTransformWrapper(const Container<Input> &self,
                         const std::function<py::object(Input)> &transform) {
  auto transformed_view = self.template GetTransformedView<py::object>(transform);
  return std::ranges::to<std::vector>(std::move(transformed_view));
}

template <CopyableType T>
auto GetIter(const Container<T> &self) {
  return py::make_iterator(self.begin(), self.end());
}

template <CopyableType T>
auto GetRepr(const Container<T> &self, std::string_view class_name) {
  return std::format("<{}(size={}) at {}>", class_name, self.GetSize(),
                     static_cast<const void *>(&self));
}

// Template function to register container bindings for a specific type
template <typename T>
void BindContainerFor(py::module &m, std::string_view type_name) {
  auto class_name = std::format("{}Container", type_name);
  py::class_<Container<T>>(m, class_name.c_str())
      .def(py::init<>())
      .def(py::init<typename Container<T>::size_type>())
      .def(py::init<std::initializer_list<T>>())
      .def(py::init<std::vector<T>>())
      .def("add", py::overload_cast<const T &>(&Container<T>::Add))
      .def("remove", &Container<T>::Remove)
      .def("size", &Container<T>::GetSize)
      .def("empty", &Container<T>::IsEmpty)
      .def("at", &GetItem<T>)
      .def("view", &Container<T>::GetView)
      .def("filter", &GetFilteredWrapper<T>)
      .def("transform", &GetTransformWrapper<T>)
      .def("__len__", &Container<T>::GetSize)
      .def("__bool__", [](const Container<T> &self) { return !self.IsEmpty(); })
      .def("__getitem__", &GetItem<T>)
      .def("__iter__", &GetIter<T>, py::keep_alive<0, 1>())
      .def("__str__", [](const Container<T> &self) { return std::format("{}", self); })
      .def("__repr__", [&](const Container<T> &self) { return GetRepr(self, class_name); });
}

}  // namespace

void BindContainers(py::module &m) {
  BindContainerFor<int>(m, "Int");
  BindContainerFor<double>(m, "Float");
  BindContainerFor<std::string>(m, "String");
}
