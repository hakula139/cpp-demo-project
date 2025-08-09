/**
 * @file shapes_binding.cpp
 * @brief Python bindings for the shapes module
 *
 * This file contains pybind11 bindings for the shapes module including
 * the Shape base class, Circle, and Rectangle classes.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "shapes/shape.hpp"

namespace py = pybind11;

void bind_shapes(py::module &m) {
  using namespace cpp_features::shapes;

  // Bind the abstract Shape base class
  py::class_<Shape>(m, "Shape", "Abstract base class for geometric shapes")
      .def("get_area", &Shape::GetArea, "Calculate the area of the shape")
      .def("get_perimeter", &Shape::GetPerimeter, "Calculate the perimeter of the shape")
      .def("draw", &Shape::Draw, "Draw the shape")
      .def("get_name", &Shape::GetName, "Get the name of the shape")
      .def("__str__", [](const Shape &s) { return std::string{s.GetName()}; })
      .def("__repr__", [](const Shape &s) {
        return std::format("<{} at {}>", s.GetName(), static_cast<const void *>(&s));
      });

  // Bind Circle class
  py::class_<Circle, Shape>(m, "Circle", "Circle shape with radius-based geometry")
      .def(py::init<double>(), py::arg("radius"), "Construct a circle with the specified radius")
      .def("get_radius", &Circle::GetRadius, "Get the radius of the circle")
      .def("__eq__", [](const Circle &self, const Circle &other) { return self == other; })
      .def("__lt__", [](const Circle &self, const Circle &other) { return (self <=> other) < 0; })
      .def("__le__", [](const Circle &self, const Circle &other) { return (self <=> other) <= 0; })
      .def("__gt__", [](const Circle &self, const Circle &other) { return (self <=> other) > 0; })
      .def("__ge__", [](const Circle &self, const Circle &other) { return (self <=> other) >= 0; })
      .def("__str__",
           [](const Circle &c) { return std::format("Circle (r = {:.2f})", c.GetRadius()); })
      .def("__repr__", [](const Circle &c) {
        return std::format("<Circle(radius={:.2f}) at {}>", c.GetRadius(),
                           static_cast<const void *>(&c));
      });

  // Bind Rectangle::Dimensions struct
  py::class_<Rectangle::Dimensions>(m, "RectangleDimensions", "Dimensions structure for rectangle")
      .def(py::init<double, double>(), py::arg("width"), py::arg("height"))
      .def_readwrite("width", &Rectangle::Dimensions::width, "Width of the rectangle")
      .def_readwrite("height", &Rectangle::Dimensions::height, "Height of the rectangle")
      .def("__str__",
           [](const Rectangle::Dimensions &d) {
             return std::format("Dimensions(width={:.2f}, height={:.2f})", d.width, d.height);
           })
      .def("__repr__", [](const Rectangle::Dimensions &d) {
        return std::format("<RectangleDimensions(width={:.2f}, height={:.2f}) at {}>", d.width,
                           d.height, static_cast<const void *>(&d));
      });

  // Bind Rectangle class
  py::class_<Rectangle, Shape>(m, "Rectangle", "Rectangle shape with width and height geometry")
      .def(py::init<Rectangle::Dimensions>(), py::arg("dimensions"),
           "Construct a rectangle from a Dimensions structure")
      .def(py::init<double, double>(), py::arg("width"), py::arg("height"),
           "Construct a rectangle with separate width and height values")
      .def(py::init<double>(), py::arg("side"),
           "Construct a square rectangle with equal width and height")
      .def("get_width", &Rectangle::GetWidth, "Get the width of the rectangle")
      .def("get_height", &Rectangle::GetHeight, "Get the height of the rectangle")
      .def("is_square", &Rectangle::IsSquare, "Check if the rectangle is a square")
      .def("__eq__", [](const Rectangle &self, const Rectangle &other) { return self == other; })
      .def("__lt__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) < 0; })
      .def("__le__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) <= 0; })
      .def("__gt__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) > 0; })
      .def("__ge__",
           [](const Rectangle &self, const Rectangle &other) { return (self <=> other) >= 0; })
      .def("__str__",
           [](const Rectangle &r) {
             return std::format("Rectangle (w = {:.2f}, h = {:.2f})", r.GetWidth(), r.GetHeight());
           })
      .def("__repr__", [](const Rectangle &r) {
        return std::format("<Rectangle(width={:.2f}, height={:.2f}) at {}>", r.GetWidth(),
                           r.GetHeight(), static_cast<const void *>(&r));
      });

  // Bind factory functions
  m.def(
      "create_circle",
      [](double radius) -> std::unique_ptr<Circle> { return CreateCircle(radius); },
      py::arg("radius"), "Factory function for creating Circle objects",
      py::return_value_policy::take_ownership);

  m.def(
      "create_rectangle",
      [](double width, double height) -> std::unique_ptr<Rectangle> {
        return CreateRectangle(width, height);
      },
      py::arg("width"), py::arg("height"), "Factory function for creating Rectangle objects",
      py::return_value_policy::take_ownership);

  m.def(
      "create_square", [](double side) -> std::unique_ptr<Rectangle> { return CreateSquare(side); },
      py::arg("side"), "Factory function for creating square Rectangle objects",
      py::return_value_policy::take_ownership);

  m.def(
      "create_shape",
      [](const std::string &shape_type, py::args args) -> std::unique_ptr<Shape> {
        if (shape_type == "circle") {
          if (args.size() != 1) {
            throw std::invalid_argument("Circle requires exactly 1 argument (radius)");
          }
          return CreateCircle(args[0].cast<double>());
        } else if (shape_type == "rectangle") {
          if (args.size() == 1) {
            return CreateSquare(args[0].cast<double>());
          } else if (args.size() == 2) {
            return CreateRectangle(args[0].cast<double>(), args[1].cast<double>());
          } else {
            throw std::invalid_argument(
                "Rectangle requires 1 argument (side) or 2 arguments (width, height)");
          }
        } else {
          throw std::invalid_argument("Unknown shape type: " + shape_type);
        }
      },
      py::arg("shape_type"), "Generic factory function for creating shapes");
}
