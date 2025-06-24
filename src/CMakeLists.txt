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

# Create main demo library that combines all components
add_library(demo_lib INTERFACE)
add_library(demo::lib ALIAS demo_lib)

target_link_libraries(demo_lib
    INTERFACE
        demo::shapes
        demo::containers
        demo::algorithms
        demo::memory
        demo::random
        demo::strings
        demo::exceptions
        demo::timing
)

target_include_directories(demo_lib
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(demo_lib
    INTERFACE
        demo::project_options
        demo::project_warnings
)

# Create main executable
add_executable(demo main.cpp)
add_executable(demo::demo ALIAS demo)

target_link_libraries(demo
    PRIVATE
        demo::lib
)

# Installation
install(TARGETS demo_lib demo
    EXPORT demo-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)