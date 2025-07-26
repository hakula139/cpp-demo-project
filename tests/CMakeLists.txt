# Tests CMakeLists.txt

# Check if Catch2 is available
if(NOT TARGET Catch2::Catch2WithMain)
  if(NOT COMMAND catch_discover_tests)
    message(
      FATAL_ERROR
      "Catch2 test discovery functions not available. Make sure Dependencies.cmake is included before tests."
    )
  endif()
endif()

add_executable(
  ${PROJECT_NAME}_tests
  test_algorithms.cpp
  test_concepts.cpp
  test_containers.cpp
  test_exceptions.cpp
  test_memory.cpp
  test_random.cpp
  test_shapes.cpp
  test_timer.cpp
)

# Link against the demo library and Catch2
target_link_libraries(
  ${PROJECT_NAME}_tests
  PRIVATE
    ${PROJECT_NAME}::lib
    Catch2::Catch2WithMain
)

# Suppress warnings from Catch2
target_compile_options(
  ${PROJECT_NAME}_tests
  PRIVATE
    -Wno-shadow
    -Wno-error=shadow
)

# Discover and register tests with CTest
catch_discover_tests(${PROJECT_NAME}_tests)
