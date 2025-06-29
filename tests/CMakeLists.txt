# Tests CMakeLists.txt

# Check if Catch2 is available
if(NOT TARGET Catch2::Catch2WithMain)
  if(NOT COMMAND catch_discover_tests)
    message(FATAL_ERROR "Catch2 test discovery functions not available. Make sure Dependencies.cmake is included before tests.")
  endif()
endif()

add_executable(demo_tests
  test_concepts.cpp
  test_containers.cpp
  test_shapes.cpp
  test_main.cpp
)

# Link against the demo library and Catch2
target_link_libraries(demo_tests
  PRIVATE
  demo::lib
  Catch2::Catch2WithMain
)

# Discover and register tests with CTest
catch_discover_tests(demo_tests)
