# Shapes module

add_library(demo_shapes
    shape.cpp
    circle.cpp
    rectangle.cpp
)

add_library(demo::shapes ALIAS demo_shapes)

target_include_directories(demo_shapes
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(demo_shapes
    PUBLIC
        demo::project_options
    PRIVATE
        demo::project_warnings
)

# Installation
install(TARGETS demo_shapes
    EXPORT demo-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)