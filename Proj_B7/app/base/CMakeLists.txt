cmake_minimum_required(VERSION 3.22)

project(base)

# ======================================
# Language requirements
# ======================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES src/*.cc)


add_library(base STATIC ${SOURCE_FILES})
target_include_directories(base PRIVATE ./include/)

# ======================================
# Vendor include directories
# ======================================
target_include_directories(${PROJECT_NAME} PUBLIC ../vendor/taskflow)
target_include_directories(${PROJECT_NAME} PUBLIC ../vendor/tomlplusplus/include)