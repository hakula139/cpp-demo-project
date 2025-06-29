# Module helper functions for demo project

include(GNUInstallDirs)

# Set configurable project prefix - defaults to the top-level project name
if(NOT DEFINED PROJECT_PREFIX)
  set(PROJECT_PREFIX "${CMAKE_PROJECT_NAME}")
endif()

#[=======================================================================[.rst:
add_demo_module
---------------

Add a demo module (header-only or with source files).

  add_demo_module(<name>
    [SOURCES <source1> [<source2> ...]]
    [DEPENDENCIES <dep1> [<dep2> ...]]
  )

Arguments:
  <name>        - Name of the module (without ${PROJECT_PREFIX}_ prefix)
  SOURCES       - Optional list of source files. If not provided, creates header-only INTERFACE library
  DEPENDENCIES  - Optional list of additional dependencies to link

This function creates:
  - ${PROJECT_PREFIX}_<name> target (INTERFACE if no SOURCES, regular library if SOURCES provided)
  - ${PROJECT_PREFIX}::<name> alias
  - Proper include directories setup
  - Links to project_options and project_warnings
  - Installation configuration

Examples:
  add_demo_module(concepts)                  # Header-only module
  add_demo_module(shapes SOURCES shape.cpp)  # Regular module with sources

Note: The actual prefix used is determined by PROJECT_PREFIX variable, which defaults to CMAKE_PROJECT_NAME.
#]=======================================================================]
function(add_demo_module MODULE_NAME)
  set(options "")
  set(oneValueArgs "")
  set(multiValueArgs SOURCES DEPENDENCIES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(TARGET_NAME "${PROJECT_PREFIX}_${MODULE_NAME}")

  # Determine if this is a header-only module based on presence of SOURCES
  if(ARG_SOURCES)
    # Create regular library with source files
    add_library(${TARGET_NAME} ${ARG_SOURCES})
    add_library(${PROJECT_PREFIX}::${MODULE_NAME} ALIAS ${TARGET_NAME})

    # Set include directories for regular library
    target_include_directories(${TARGET_NAME}
      PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
      PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Link to common project targets for regular library
    target_link_libraries(${TARGET_NAME}
      PUBLIC
      ${PROJECT_PREFIX}::project_options
      PRIVATE
      ${PROJECT_PREFIX}::project_warnings
    )

    # Link additional dependencies if provided
    if(ARG_DEPENDENCIES)
      target_link_libraries(${TARGET_NAME}
        PUBLIC ${ARG_DEPENDENCIES}
      )
    endif()
  else()
    # Create header-only INTERFACE library
    add_library(${TARGET_NAME} INTERFACE)
    add_library(${PROJECT_PREFIX}::${MODULE_NAME} ALIAS ${TARGET_NAME})

    # Set include directories for header-only library
    target_include_directories(${TARGET_NAME}
      INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # Link to common project targets as INTERFACE (for header-only libraries)
    target_link_libraries(${TARGET_NAME}
      INTERFACE
      ${PROJECT_PREFIX}::project_options
      ${PROJECT_PREFIX}::project_warnings
    )

    # Link additional dependencies if provided
    if(ARG_DEPENDENCIES)
      target_link_libraries(${TARGET_NAME}
        INTERFACE ${ARG_DEPENDENCIES}
      )
    endif()
  endif()

  # Installation (same for both types)
  install(TARGETS ${TARGET_NAME}
    EXPORT ${PROJECT_PREFIX}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endfunction()
