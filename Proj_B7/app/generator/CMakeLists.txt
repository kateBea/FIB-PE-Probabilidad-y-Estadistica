cmake_minimum_required(VERSION 3.22)

project(generator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES src/*.cc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE base)

target_include_directories(${PROJECT_NAME} PRIVATE ./include)
target_include_directories(${PROJECT_NAME} PRIVATE ../base/include)