cmake_minimum_required(VERSION 3.1)

project(Refureku)

message("======================================")
message("Here is the project source dir : ${PROJECT_SOURCE_DIR}")
message("Build type ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY Libraries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Binaries)

include_directories(Include)
include_directories(Libraries/Include)

add_executable(exe Source/main.cpp)

