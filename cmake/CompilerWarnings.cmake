# Compiler warnings module
# Based on https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md

function(set_project_warnings project_name)
    option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" FALSE)

    set(CLANG_WARNINGS
        -Weverything # Enable all warnings for strict rules
        -Wno-c++98-compat # Disable C++98 compatibility warnings (we want modern C++)
        -Wno-c++98-compat-pedantic # Disable pedantic C++98 compatibility warnings
        -Wno-padded # Disable padding warnings (usually not actionable)
        -Wno-weak-vtables # Disable weak vtables warning (often not relevant)
    )

    if(WARNINGS_AS_ERRORS)
        list(APPEND CLANG_WARNINGS -Werror)
    endif()

    set(GCC_WARNINGS
        -Wall
        -Wextra # Reasonable and standard
        -Wcast-align # Warn for potential performance problem casts
        -Wconversion # Warn on type conversions that may lose data
        -Wdouble-promotion # Warn if float is implicit promoted to double
        -Wduplicated-branches # Warn if if / else branches have duplicated code
        -Wduplicated-cond # Warn if if / else chain has duplicated conditions
        -Wformat=2 # Warn on security issues around functions that format output (i.e. printf)
        -Wimplicit-fallthrough # Warn on statements that fallthrough without an explicit annotation
        -Wlogical-op # Warn about logical operations being used where bitwise were probably wanted
        -Wmisleading-indentation # Warn if indentation implies blocks where blocks do not exist
        -Wnon-virtual-dtor # Warn the user if a class with virtual functions has a non-virtual destructor
        -Wnull-dereference # Warn if a null dereference is detected
        -Wold-style-cast # Warn for C-style casts
        -Woverloaded-virtual # Warn if you overload (not override) a virtual function
        -Wpedantic # Warn if non-standard C++ is used
        -Wshadow # Warn the user if a variable declaration shadows one from a parent context
        -Wsign-conversion # Warn on sign conversions
        -Wunused # Warn on anything being unused
        -Wuseless-cast # Warn if you perform a cast to the same type
    )

    if(WARNINGS_AS_ERRORS)
        list(APPEND GCC_WARNINGS -Werror)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
    else()
        message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()

    # Use the same warning flags for C
    set(PROJECT_WARNINGS_C "${PROJECT_WARNINGS_CXX}")

    target_compile_options(
        ${project_name}
        INTERFACE
            # C++ warnings
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS_CXX}>
            # C warnings
            $<$<COMPILE_LANGUAGE:C>:${PROJECT_WARNINGS_C}>
    )
endfunction()
