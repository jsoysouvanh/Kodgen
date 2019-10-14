cmake_minimum_required(VERSION 3.1)

project(Refureku)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY Libraries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Binaries)

include_directories(Include)
include_directories(Libraries/Include)

add_executable(exe Source/main.cpp)

message("--------------------------------")
message("Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")