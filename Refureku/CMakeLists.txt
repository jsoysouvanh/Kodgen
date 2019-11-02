cmake_minimum_required(VERSION 3.8)

message("Cmake:		${CMAKE_VERSION}")
message("Compiler:	${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

project(Refureku)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Binaries)

include_directories(Include)
include_directories(Libraries/Include)

link_directories(Libraries/x64/Static)

add_executable(refureku Source/main.cpp)

set_target_properties(refureku
						PROPERTIES
						CXX_STANDARD 17
						CXX_STANDARD_REQUIRED ON
						CXX_EXTENSIONS OFF
					 )

target_link_libraries(refureku general libclang)
