# Static analysis tools configuration

option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option)
        message(STATUS "Clang-Tidy finished setting up.")
    else()
        message(WARNING "clang-tidy requested but executable not found")
    endif()
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK
            ${CPPCHECK}
            --suppress=missingInclude
            --enable=all
            --inline-suppr
            --inconclusive)
        message(STATUS "Cppcheck finished setting up.")
    else()
        message(WARNING "cppcheck requested but executable not found")
    endif()
endif()