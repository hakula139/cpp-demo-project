# Examples CMakeLists.txt

# Example showing how to use individual components
add_executable(algorithms_example algorithms_example.cpp)
target_link_libraries(algorithms_example
  PRIVATE
  ${PROJECT_NAME}::lib
)

add_executable(containers_example containers_example.cpp)
target_link_libraries(containers_example
  PRIVATE
  ${PROJECT_NAME}::lib
)

add_executable(shapes_example shapes_example.cpp)
target_link_libraries(shapes_example
  PRIVATE
  ${PROJECT_NAME}::lib
)

# Example showing how to use the complete library
add_executable(full_example full_example.cpp)
target_link_libraries(full_example
  PRIVATE
  ${PROJECT_NAME}::lib
)
