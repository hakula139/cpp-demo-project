# Examples

# Example showing how to use individual components
add_executable(containers_example containers_example.cpp)
target_link_libraries(containers_example
    PRIVATE
        demo::containers
        demo::project_warnings
)

add_executable(shapes_example shapes_example.cpp)
target_link_libraries(shapes_example
    PRIVATE
        demo::shapes
        demo::project_warnings
)

# Example showing how to use the complete library
add_executable(full_demo_example full_demo_example.cpp)
target_link_libraries(full_demo_example
    PRIVATE
        demo::lib
        demo::project_warnings
)
