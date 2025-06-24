# Strings module

add_library(demo_strings
    string_ops.cpp
)

add_library(demo::strings ALIAS demo_strings)

target_include_directories(demo_strings
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(demo_strings
    PUBLIC
        demo::project_options
    PRIVATE
        demo::project_warnings
)

# Installation
install(TARGETS demo_strings
    EXPORT demo-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)