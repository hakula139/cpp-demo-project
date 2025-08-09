# Python bindings for cpp-demo-project
# This directory contains pybind11 bindings for all C++ modules

cmake_minimum_required(VERSION 3.23)

# Ensure pybind11 is available
if(NOT TARGET pybind11::pybind11)
  message(
    FATAL_ERROR
    "pybind11 not found. Enable BUILD_PYTHON_BINDINGS and ensure pybind11 is available."
  )
endif()

# Create the main Python module
pybind11_add_module(cpp_features
  cpp_features.cpp
  shapes_binding.cpp
  containers_binding.cpp
  algorithms_binding.cpp
  exceptions_binding.cpp
  memory_binding.cpp
  timing_binding.cpp
  random_binding.cpp
)

# Link against all the required libraries
target_link_libraries(cpp_features PRIVATE demo::lib)

# Include directories
target_include_directories(
  cpp_features
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Set properties for the Python module
set_target_properties(
  cpp_features
  PROPERTIES
    CXX_STANDARD
      23
    CXX_STANDARD_REQUIRED
      ON
    CXX_VISIBILITY_PRESET
      hidden
    VISIBILITY_INLINES_HIDDEN
      ON
)

# Compile definitions
target_compile_definitions(cpp_features PRIVATE VERSION_INFO=${PROJECT_VERSION})

# Install the Python module
install(TARGETS cpp_features COMPONENT python LIBRARY DESTINATION .)
