cmake_minimum_required(VERSION 3.13.5)

set(PropertyRulesTestsTarget Refureku_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE}_PropertyRulesTests)
set(ParsingTestsTarget Refureku_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE}_ParsingTests)
set(FileGeneratorTestsTarget Refureku_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE}_FileGeneratorTests)

add_executable(${PropertyRulesTestsTarget} PropertyRulesTests.cpp)
add_executable(${ParsingTestsTarget} ParsingTests.cpp)
add_executable(${FileGeneratorTestsTarget} FileGeneratorTests.cpp)

# Setup language requirements
set_target_properties(${PropertyRulesTestsTarget} ${ParsingTestsTarget} ${FileGeneratorTestsTarget}
						PROPERTIES
						CXX_STANDARD 17
						CXX_STANDARD_REQUIRED ON
						CXX_EXTENSIONS OFF)

# Link to Refureku library to test it
target_link_libraries(${PropertyRulesTestsTarget} PRIVATE ${RefurekuTargetLibrary})
target_link_libraries(${ParsingTestsTarget} PRIVATE ${RefurekuTargetLibrary})
target_link_libraries(${FileGeneratorTestsTarget} PRIVATE ${RefurekuTargetLibrary})

add_dependencies(${PropertyRulesTestsTarget} ${RefurekuTargetLibrary})
add_dependencies(${ParsingTestsTarget} ${RefurekuTargetLibrary})
add_dependencies(${FileGeneratorTestsTarget} ${RefurekuTargetLibrary})

add_test(NAME ${PropertyRulesTestsTarget} COMMAND ${PropertyRulesTestsTarget})
add_test(NAME ${ParsingTestsTarget} COMMAND ${ParsingTestsTarget})
add_test(NAME ${FileGeneratorTestsTarget} COMMAND ${FileGeneratorTestsTarget})