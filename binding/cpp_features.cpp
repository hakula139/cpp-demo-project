/**
 * @file cpp_features.cpp
 * @brief Main pybind11 module
 *
 * This file contains the main pybind11 module definition that brings together all the individual
 * component bindings into a single Python module.
 */

#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Forward declarations for binding functions
void BindAlgorithms(py::module &m);
void BindContainers(py::module &m);
void BindExceptions(py::module &m);
void BindRandom(py::module &m);
void BindShapes(py::module &m);
void BindTiming(py::module &m);

PYBIND11_MODULE(cpp_features, m) {
  m.doc() = "Python wrappers for the C++ demo project";
  m.attr("__version__") = PYBIND11_STRINGIFY(VERSION_INFO);

  auto algorithms = m.def_submodule("algorithms");
  BindAlgorithms(algorithms);

  auto containers = m.def_submodule("containers");
  BindContainers(containers);

  auto exceptions = m.def_submodule("exceptions");
  BindExceptions(exceptions);

  auto random = m.def_submodule("random");
  BindRandom(random);

  auto shapes = m.def_submodule("shapes");
  BindShapes(shapes);

  auto timing = m.def_submodule("timing");
  BindTiming(timing);
}
