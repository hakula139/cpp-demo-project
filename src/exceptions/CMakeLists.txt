# Exceptions module

add_library(demo_exceptions
    custom_exception.cpp
)

add_library(demo::exceptions ALIAS demo_exceptions)

target_include_directories(demo_exceptions
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(demo_exceptions
    PUBLIC
        demo::project_options
    PRIVATE
        demo::project_warnings
)

# Installation
install(TARGETS demo_exceptions
    EXPORT demo-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)