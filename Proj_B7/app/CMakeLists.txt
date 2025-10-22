cmake_minimum_required(VERSION 3.22)

project(estadistica_B7)

# ======================================
# Language requirements
# ======================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======================================
# Linux configurations
# ======================================
if(LINUX)
    message("Target platform Linux")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g3 -D_GLIBCXX_DEBUG -Wall -Wextra -Wpedantic)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()

    # Easiest for now to run valgrind tests
endif()

# ===========================
# Add applications
# ===========================
add_subdirectory(base)
add_subdirectory(tester)
add_subdirectory(generator)
