# Containers module

add_library(demo_containers
    container.cpp
)

add_library(demo::containers ALIAS demo_containers)

target_include_directories(demo_containers
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(demo_containers
    PUBLIC
        demo::project_options
    PRIVATE
        demo::project_warnings
)

# Installation
install(TARGETS demo_containers
    EXPORT demo-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)