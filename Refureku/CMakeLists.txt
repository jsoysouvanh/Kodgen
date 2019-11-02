cmake_minimum_required(VERSION 3.1)

project(Refureku)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Binaries)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(Include)
include_directories(Libraries/Include)

link_directories(Libraries/x64/Static)

add_executable(refureku Source/main.cpp)

target_link_libraries(refureku general libclang)

message("--------------------------------")
message("Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")