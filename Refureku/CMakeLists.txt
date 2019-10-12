cmake_minimum_required(VERSION 2.8)

project(Refureku)

message("======================================")
message("Here is the project source dir : ${PROJECT_SOURCE_DIR}")

include_directories(Include)
include_directories(Libraries/Include)

add_executable(exe Source/main.cpp)

