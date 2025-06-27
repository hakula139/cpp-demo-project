# Tests configuration

# Include Catch2 CMake integration
include(CTest)

# Verify that Catch2 test discovery is available
if(NOT COMMAND catch_discover_tests)
    message(FATAL_ERROR "Catch2 test discovery functions not available. Make sure Dependencies.cmake is included before tests.")
endif()

# Test sources
set(TEST_SOURCES
    test_shapes.cpp
    test_containers.cpp
    test_concepts.cpp
    test_main.cpp
)

# Create test executable
add_executable(demo_tests ${TEST_SOURCES})
add_executable(demo::tests ALIAS demo_tests)

# Link against the demo library and test framework
target_link_libraries(demo_tests
    PRIVATE
        demo::lib
        demo::project_warnings
        Catch2::Catch2WithMain
)

# Auto-discover tests
catch_discover_tests(demo_tests)
