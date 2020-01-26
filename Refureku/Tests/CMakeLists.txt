cmake_minimum_required(VERSION 3.10)

set(PropertyRulesTestsTargetName Refureku_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE}_PropertyRulesTests)
set(ParsingTestsTargetName Refureku_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE}_ParsingTests)
set(FileGeneratorTestsTargetName Refureku_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE}_FileGeneratorTests)

add_executable(${PropertyRulesTestsTargetName} PropertyRulesTests.cpp)
add_executable(${ParsingTestsTargetName} ParsingTests.cpp)
add_executable(${FileGeneratorTestsTargetName} FileGeneratorTests.cpp)

# Setup language requirements
set_target_properties(${PropertyRulesTestsTargetName} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
set_target_properties(${ParsingTestsTargetName} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
set_target_properties(${FileGeneratorTestsTargetName} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)

# Link to Refureku library to test it
target_link_libraries(${PropertyRulesTestsTargetName} PRIVATE ${TargetLibraryName})
target_link_libraries(${ParsingTestsTargetName} PRIVATE ${TargetLibraryName})
target_link_libraries(${FileGeneratorTestsTargetName} PRIVATE ${TargetLibraryName})

add_test(NAME ${PropertyRulesTestsTargetName} COMMAND ${PropertyRulesTestsTargetName})
add_test(NAME ${ParsingTestsTargetName} COMMAND ${ParsingTestsTargetName})
add_test(NAME ${FileGeneratorTestsTargetName} COMMAND ${FileGeneratorTestsTargetName})