# Source directory CMakeLists.txt

# Create individual component libraries
add_subdirectory(algorithms)
add_subdirectory(concepts)
add_subdirectory(containers)
add_subdirectory(exceptions)
add_subdirectory(memory)
add_subdirectory(random)
add_subdirectory(shapes)
add_subdirectory(timing)

# Create main library that combines all components
add_library(${PROJECT_NAME}_lib INTERFACE)
add_library(${PROJECT_NAME}::lib ALIAS ${PROJECT_NAME}_lib)

target_link_libraries(
  ${PROJECT_NAME}_lib
  INTERFACE
    ${PROJECT_NAME}::algorithms
    ${PROJECT_NAME}::concepts
    ${PROJECT_NAME}::containers
    ${PROJECT_NAME}::exceptions
    ${PROJECT_NAME}::memory
    ${PROJECT_NAME}::random
    ${PROJECT_NAME}::shapes
    ${PROJECT_NAME}::timing
)

target_include_directories(
  ${PROJECT_NAME}_lib
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(
  ${PROJECT_NAME}_lib
  INTERFACE
    ${PROJECT_NAME}::project_options
    ${PROJECT_NAME}::project_warnings
)
