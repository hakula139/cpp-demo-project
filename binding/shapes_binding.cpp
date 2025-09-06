/**
 * @file shapes_binding.cpp
 * @brief Python bindings for the shapes module
 */

#include <format>
#include <memory>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "shapes/shape.hpp"

namespace py = pybind11;
using namespace cpp_features::shapes;

void BindShapes(py::module &m) {
  // Bind the abstract Shape base class
  py::class_<Shape, std::shared_ptr<Shape>>(m, "Shape")
      .def("get_area", &Shape::GetArea)
      .def("get_perimeter", &Shape::GetPerimeter)
      .def("draw", &Shape::Draw)
      .def("__str__", [](const Shape &self) { return std::string{self.GetName()}; })
      .def("__repr__", [](const Shape &self) {
        return std::format("<{} at {}>", self.GetName(), static_cast<const void *>(&self));
      });

  // Bind Circle class
  py::class_<Circle, Shape, std::shared_ptr<Circle>>(m, "Circle")
      .def(py::init<double>())
      .def("get_radius", &Circle::GetRadius)
      .def("__eq__", [](const Circle &self, const Circle &other) { return self == other; })
      .def("__ne__", [](const Circle &self, const Circle &other) { return self != other; })
      .def("__lt__", [](const Circle &self, const Circle &other) { return (self <=> other) < 0; })
      .def("__le__", [](const Circle &self, const Circle &other) { return (self <=> other) <= 0; })
      .def("__gt__", [](const Circle &self, const Circle &other) { return (self <=> other) > 0; })
      .def("__ge__", [](const Circle &self, const Circle &other) { return (self <=> other) >= 0; })
      .def("__str__", [](const Circle &self) { return std::format("{}", self); })
      .def("__repr__", [](const Circle &self) {
        return std::format("<Circle(radius={:.2f}) at {}>", self.GetRadius(),
                           static_cast<const void *>(&self));
      });

  // Bind Rectangle class
  py::class_<Rectangle, Shape, std::shared_ptr<Rectangle>>(m, "Rectangle")
      .def(py::init<double, double>())
      .def(py::init<double>())
      .def("get_width", &Rectangle::GetWidth)
      .def("get_height", &Rectangle::GetHeight)
      .def("is_square", &Rectangle::IsSquare)
      .def("__eq__", [](const Rectangle &self, const Rectangle &other) { return self == other; })
      .def("__ne__", [](const Rectangle &self, const Rectangle &other) { return self != other; })
      .def("__lt__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) < 0; })
      .def("__le__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) <= 0; })
      .def("__gt__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) > 0; })
      .def("__ge__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) >= 0; })
      .def("__str__", [](const Rectangle &r) { return std::format("{}", r); })
      .def("__repr__", [](const Rectangle &r) {
        return std::format("<Rectangle(width={:.2f}, height={:.2f}) at {}>", r.GetWidth(),
                           r.GetHeight(), static_cast<const void *>(&r));
      });
}
