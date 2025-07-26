# Module helper functions for project

include(GNUInstallDirs)

#[=======================================================================[.rst:
add_module
---------------

Add a module (header-only or with source files).

  add_module(<name>
    [SOURCES <source1> [<source2> ...]]
    [DEPENDENCIES <dep1> [<dep2> ...]]
  )

Arguments:
  <name>        - Name of the module (without ${PROJECT_NAME}_ prefix)
  SOURCES       - Optional list of source files. If not provided, creates header-only INTERFACE library
  DEPENDENCIES  - Optional list of additional dependencies to link

This function creates:
  - ${PROJECT_NAME}_<name> target (INTERFACE if no SOURCES, regular library if SOURCES provided)
  - ${PROJECT_NAME}::<name> alias
  - Proper include directories setup
  - Links to project_options and project_warnings
  - Installation configuration

Examples:
  add_module(concepts)                  # Header-only module
  add_module(shapes SOURCES shape.cpp)  # Regular module with sources
#]=======================================================================]
function(add_module MODULE_NAME)
  set(options "")
  set(oneValueArgs "")
  set(
    multiValueArgs
    SOURCES
    DEPENDENCIES
  )
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(TARGET_NAME "${PROJECT_NAME}_${MODULE_NAME}")

  # Determine if this is a header-only module based on presence of SOURCES
  if(ARG_SOURCES)
    # Create regular library with source files
    add_library(${TARGET_NAME} ${ARG_SOURCES})
    add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${TARGET_NAME})

    # Set include directories for regular library
    target_include_directories(
      ${TARGET_NAME}
      PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Link to common project targets for regular library
    target_link_libraries(
      ${TARGET_NAME}
      PUBLIC
        ${PROJECT_NAME}::project_options
      PRIVATE
        ${PROJECT_NAME}::project_warnings
    )

    # Link additional dependencies if provided
    if(ARG_DEPENDENCIES)
      target_link_libraries(${TARGET_NAME} PUBLIC ${ARG_DEPENDENCIES})
    endif()
  else()
    # Create header-only INTERFACE library
    add_library(${TARGET_NAME} INTERFACE)
    add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${TARGET_NAME})

    # Set include directories for header-only library
    target_include_directories(
      ${TARGET_NAME}
      INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # Link to common project targets as INTERFACE for header-only library
    target_link_libraries(
      ${TARGET_NAME}
      INTERFACE
        ${PROJECT_NAME}::project_options
        ${PROJECT_NAME}::project_warnings
    )

    # Link additional dependencies if provided
    if(ARG_DEPENDENCIES)
      target_link_libraries(${TARGET_NAME} INTERFACE ${ARG_DEPENDENCIES})
    endif()
  endif()

  # Installation (same for both types)
  install(
    TARGETS
      ${TARGET_NAME}
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME
      DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endfunction()
