# Examples CMakeLists.txt

add_executable(algorithms_example algorithms_example.cpp)
target_link_libraries(algorithms_example PRIVATE ${PROJECT_NAME}::lib)

add_executable(containers_example containers_example.cpp)
target_link_libraries(containers_example PRIVATE ${PROJECT_NAME}::lib)

add_executable(exceptions_example exceptions_example.cpp)
target_link_libraries(exceptions_example PRIVATE ${PROJECT_NAME}::lib)

add_executable(memory_example memory_example.cpp)
target_link_libraries(memory_example PRIVATE ${PROJECT_NAME}::lib)

add_executable(random_example random_example.cpp)
target_link_libraries(random_example PRIVATE ${PROJECT_NAME}::lib)

add_executable(shapes_example shapes_example.cpp)
target_link_libraries(shapes_example PRIVATE ${PROJECT_NAME}::lib)

add_executable(timer_example timer_example.cpp)
target_link_libraries(timer_example PRIVATE ${PROJECT_NAME}::lib)
