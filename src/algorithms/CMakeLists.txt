# Algorithms module

add_library(demo_algorithms
    stl_demos.cpp
)

add_library(demo::algorithms ALIAS demo_algorithms)

target_include_directories(demo_algorithms
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(demo_algorithms
    PUBLIC
        demo::project_options
    PRIVATE
        demo::project_warnings
)

# Installation
install(TARGETS demo_algorithms
    EXPORT demo-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)