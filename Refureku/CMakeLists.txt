cmake_minimum_required(VERSION 3.8)

project(Refureku)

set(IS_SUPPORTED_COMPILER NO)
set(IS_CLANG_OR_GCC NO)

# Handle specific flags for each supported compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	
	add_compile_options(-stdlib=libc++)
	
	set(IS_SUPPORTED_COMPILER YES)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	set(IS_CLANG_OR_GCC YES)
	set(IS_SUPPORTED_COMPILER YES)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

	set(IS_CLANG_OR_GCC YES)
	set(IS_SUPPORTED_COMPILER YES)

endif()

if (IS_SUPPORTED_COMPILER)
	# Information messages about cmake version and compiler
	message("Cmake:		${CMAKE_VERSION}")
	message("Compiler:	${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

	# Setup include directories
	include_directories(Include)
	include_directories(Libraries/Include)

	# Setup link directories
	link_directories(Libraries/x64/Static)

	# Setup output directory
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Binaries/${CMAKE_BUILD_TYPE})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Binaries/${CMAKE_BUILD_TYPE})

	# Setup warning level
	if (MSVC)
		add_compile_options(/W4)
	else()	# Clang or GCC
		add_compile_options(-Wall -Wextra -Wpedantic)
	endif()

	# Setup executable / lib
	add_executable(refureku Source/main.cpp)

	# Link to required libraries
	target_link_libraries(refureku PRIVATE libclang)
	target_link_libraries(refureku PRIVATE -lc++abi)

	# Setup language requirements
	set_target_properties(refureku
						PROPERTIES
						CXX_STANDARD 17
						CXX_STANDARD_REQUIRED ON
						CXX_EXTENSIONS OFF
					 )

else()
	message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} is not supported for this project, please use Clang, GCC or MSVC")
endif()
