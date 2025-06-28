# Static analysis tools configuration

option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY clang-tidy)
    if(CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
        message(STATUS "clang-tidy finished setting up")
    else()
        message(WARNING "clang-tidy requested but executable not found")
    endif()
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK
            ${CPPCHECK}
            --enable=all
            --inconclusive
            --suppress=mismatchingContainerExpression
            --suppress=missingIncludeSystem
            --suppress=unusedFunction
        )
        message(STATUS "cppcheck finished setting up")
    else()
        message(WARNING "cppcheck requested but executable not found")
    endif()
endif()
