cmake_minimum_required(VERSION 3.22)

# ======================================
# Language requirements
# ======================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES src/*.cc)

add_library(base STATIC ${SOURCE_FILES})
target_include_directories(base PRIVATE ./include/)
