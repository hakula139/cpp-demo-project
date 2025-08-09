/**
 * @file memory_binding.cpp
 * @brief Python bindings for the memory module
 */

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "memory/resource_manager.hpp"
#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"

namespace py = pybind11;

void bind_memory(py::module &m) {
  using namespace cpp_features::memory;
  using namespace cpp_features::shapes;

  // Bind ResourceManager
  py::class_<ResourceManager>(m, "ResourceManager")
      .def(py::init<>())
      .def(
          "create_unique_circle",
          [](ResourceManager &self, double radius) { return self.CreateUnique<Circle>(radius); },
          py::return_value_policy::take_ownership)
      .def(
          "create_unique_rectangle",
          [](ResourceManager &self, double width, double height) {
            return self.CreateUnique<Rectangle>(width, height);
          },
          py::return_value_policy::take_ownership)
      .def("create_shared_circle",
           [](ResourceManager &self, double radius) { return self.CreateShared<Circle>(radius); })
      .def("create_shared_rectangle",
           [](ResourceManager &self, double width, double height) {
             return self.CreateShared<Rectangle>(width, height);
           })
      .def("register_cleanup",
           [](ResourceManager &self, std::function<void()> cleanup_func) {
             self.RegisterCleanup<int>(cleanup_func);
           })
      .def("execute_cleanup", &ResourceManager::ExecuteCleanup)
      .def("execute_cleanup_noexcept", &ResourceManager::ExecuteCleanupNoexcept)
      .def("disable_automatic_cleanup", &ResourceManager::DisableAutomaticCleanup)
      .def("__repr__", [](const ResourceManager &rm) {
        return std::format("<ResourceManager at {}>", static_cast<const void *>(&rm));
      });
}
