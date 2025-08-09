/**
 * @file cpp_features.cpp
 * @brief Main Python module for cpp-demo-project
 *
 * This file contains the main pybind11 module definition that brings together
 * all the individual component bindings into a single Python module.
 */

#include <pybind11/pybind11.h>

namespace py = pybind11;

// Forward declarations for binding functions
void bind_shapes(py::module &m);
void bind_containers(py::module &m);
void bind_algorithms(py::module &m);
void bind_exceptions(py::module &m);
void bind_memory(py::module &m);
void bind_timing(py::module &m);
void bind_random(py::module &m);

PYBIND11_MODULE(cpp_features, m) {
  m.doc() = "Python bindings for the modern C++23 features demo project";
  m.attr("__version__") = PYBIND11_STRINGIFY(VERSION_INFO);

  // Create submodules for each component
  auto shapes =
      m.def_submodule("shapes", "Geometric shapes with polymorphism and factory functions");
  bind_shapes(shapes);

  auto containers =
      m.def_submodule("containers", "Modern container with concepts and ranges support");
  bind_containers(containers);

  auto algorithms = m.def_submodule("algorithms", "STL algorithm wrappers with concepts");
  bind_algorithms(algorithms);

  auto exceptions =
      m.def_submodule("exceptions", "Error handling with Result type and custom exceptions");
  bind_exceptions(exceptions);

  auto memory = m.def_submodule("memory", "Resource management and RAII utilities");
  bind_memory(memory);

  auto timing = m.def_submodule("timing", "High-resolution timing and benchmarking utilities");
  bind_timing(timing);

  auto random = m.def_submodule("random", "Type-safe random number generation");
  bind_random(random);
}
