# Source directory CMakeLists.txt

# Create individual component libraries
add_subdirectory(shapes)
add_subdirectory(containers)
add_subdirectory(algorithms)
add_subdirectory(memory)
add_subdirectory(random)
add_subdirectory(strings)
add_subdirectory(exceptions)
add_subdirectory(timing)
add_subdirectory(concepts)

# Create main demo library that combines all components
add_library(${PROJECT_NAME}_lib INTERFACE)
add_library(${PROJECT_NAME}::lib ALIAS ${PROJECT_NAME}_lib)

target_link_libraries(${PROJECT_NAME}_lib
  INTERFACE
  ${PROJECT_NAME}::shapes
  ${PROJECT_NAME}::containers
  ${PROJECT_NAME}::algorithms
  ${PROJECT_NAME}::memory
  ${PROJECT_NAME}::random
  ${PROJECT_NAME}::strings
  ${PROJECT_NAME}::exceptions
  ${PROJECT_NAME}::timing
  ${PROJECT_NAME}::concepts
)

target_include_directories(${PROJECT_NAME}_lib
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME}_lib
  INTERFACE
  ${PROJECT_NAME}::project_options
  ${PROJECT_NAME}::project_warnings
)

# Create main executable
add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  ${PROJECT_NAME}::lib
)

# Installation
install(TARGETS ${PROJECT_NAME}_lib ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
