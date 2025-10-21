cmake_minimum_required(VERSION 3.22)

project(estadistica_B7)

# ======================================
# Language requirements
# ======================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===========================
# Add applications
# ===========================
add_subdirectory(base)
add_subdirectory(tester)
add_subdirectory(generator)
