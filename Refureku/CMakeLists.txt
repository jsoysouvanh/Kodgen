cmake_minimum_required(VERSION 3.10)

project(RefurekuLibrary)

# Handle specific flags for each supported compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	
	add_compile_options(-stdlib=libc++)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	
	# Nothing at the moment

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

	# Nothing at the moment

else()
	
	message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} is not supported for this project, please use Clang, GCC or MSVC")

endif()

# Information messages about cmake version and compiler
message("Cmake:		${CMAKE_VERSION}")
message("Compiler:	${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Setup include directories
include_directories(Include)
include_directories(Libraries/Include)

# Setup link directories
link_directories(Libraries/x64/Static)

# Setup output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Lib/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})	# .dll / .so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY Lib/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})	# .lib / .a
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Bin/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})	# .exe

# Setup warning level
if (MSVC)
	add_compile_options(/W4)
else()	# Clang or GCC
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(TargetLibraryName Refureku_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE})

add_library(${TargetLibraryName} STATIC
				Source/Properties/SimplePropertyRule.cpp
				Source/Properties/ComplexPropertyRule.cpp
				Source/Properties/ComplexProperty.cpp
				Source/Properties/PropertyRules.cpp
				Source/Properties/PropertyParser.cpp
				Source/Properties/PropertyGroup.cpp
	
				Source/InfoStructures/EntityInfo.cpp
				Source/InfoStructures/FieldInfo.cpp
				Source/InfoStructures/MethodInfo.cpp
				Source/InfoStructures/StructClassInfo.cpp
				Source/InfoStructures/EnumInfo.cpp
				Source/InfoStructures/EnumValueInfo.cpp
				Source/InfoStructures/ParsingInfo.cpp
				Source/InfoStructures/TypeInfo.cpp
	
				Source/Parsing/EParsingError.cpp
				Source/Parsing/ParsingError.cpp
				Source/Parsing/ClassParser.cpp
				Source/Parsing/FieldParser.cpp
				Source/Parsing/MethodParser.cpp
				Source/Parsing/EnumParser.cpp
				Source/Parsing/EnumValueParser.cpp
				Source/Parsing/Parser.cpp
	
				Source/Misc/EAccessSpecifier.cpp
				Source/Misc/Helpers.cpp
	
				Source/CodeGen/FileGenerator.cpp
				Source/CodeGen/GeneratedCodeTemplate.cpp
				Source/CodeGen/EFileGenerationError.cpp
				Source/CodeGen/FileGenerationError.cpp
				)

# Setup language requirements
set_target_properties(${TargetLibraryName}
					PROPERTIES
					CXX_STANDARD 17
					CXX_STANDARD_REQUIRED ON
					CXX_EXTENSIONS OFF
					)

# Link to required libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${TargetLibraryName} PRIVATE
							$<$<CXX_COMPILER_ID:Clang>:c++>
							$<$<CXX_COMPILER_ID:Clang>:c++abi>
							clang
							clangAST
							clangBasic
							clangDriver
							clangFrontend
							clangIndex
							clangLex
							clangSema
							clangSerialization
							clangTooling
							clangARCMigrate
							LLVMAArch64CodeGen
							LLVMAArch64AsmParser
							LLVMAArch64Desc
							LLVMAArch64Disassembler
							LLVMAArch64Info
							LLVMAArch64Utils
							LLVMAMDGPUCodeGen
							LLVMAMDGPUAsmParser
							LLVMAMDGPUDesc
							LLVMAMDGPUDisassembler
							LLVMAMDGPUInfo
							LLVMAMDGPUUtils
							LLVMARMCodeGen
							LLVMARMAsmParser
							LLVMARMDesc
							LLVMARMDisassembler
							LLVMARMInfo
							LLVMARMUtils
							LLVMBPFCodeGen
							LLVMBPFAsmParser
							LLVMBPFDesc
							LLVMBPFDisassembler
							LLVMBPFInfo
							LLVMHexagonCodeGen
							LLVMHexagonAsmParser
							LLVMHexagonDesc
							LLVMHexagonDisassembler
							LLVMHexagonInfo
							LLVMLanaiCodeGen
							LLVMLanaiAsmParser
							LLVMLanaiDesc
							LLVMLanaiDisassembler
							LLVMLanaiInfo
							LLVMMipsCodeGen
							LLVMMipsAsmParser
							LLVMMipsDesc
							LLVMMipsDisassembler
							LLVMMipsInfo
							LLVMMSP430CodeGen
							LLVMMSP430AsmParser
							LLVMMSP430Desc
							LLVMMSP430Disassembler
							LLVMMSP430Info
							LLVMNVPTXCodeGen
							LLVMNVPTXDesc
							LLVMNVPTXInfo
							LLVMPowerPCCodeGen
							LLVMPowerPCAsmParser
							LLVMPowerPCDesc
							LLVMPowerPCDisassembler
							LLVMPowerPCInfo
							LLVMRISCVCodeGen
							LLVMRISCVAsmParser
							LLVMRISCVDesc
							LLVMRISCVDisassembler
							LLVMRISCVInfo
							LLVMRISCVUtils
							LLVMSparcCodeGen
							LLVMSparcAsmParser
							LLVMSparcDesc
							LLVMSparcDisassembler
							LLVMSparcInfo
							LLVMSystemZCodeGen
							LLVMSystemZAsmParser
							LLVMSystemZDesc
							LLVMSystemZDisassembler
							LLVMSystemZInfo
							LLVMWebAssemblyCodeGen
							LLVMWebAssemblyAsmParser
							LLVMWebAssemblyDesc
							LLVMWebAssemblyDisassembler
							LLVMWebAssemblyInfo
							LLVMX86CodeGen
							LLVMX86AsmParser
							LLVMX86Desc
							LLVMX86Disassembler
							LLVMX86Info
							LLVMX86Utils
							LLVMXCoreCodeGen
							LLVMXCoreDesc
							LLVMXCoreDisassembler
							LLVMXCoreInfo
							LLVMCore
							LLVMSupport
							clangFormat
							clangToolingInclusions
							clangToolingCore
							clangFrontend
							clangDriver
							LLVMOption
							clangParse
							clangSerialization
							clangSema
							clangEdit
							clangRewrite
							clangAnalysis
							clangASTMatchers
							clangAST
							clangLex
							clangBasic
							LLVMMIRParser
							LLVMipo
							LLVMVectorize
							LLVMIRReader
							LLVMAsmParser
							LLVMInstrumentation
							LLVMLinker
							LLVMGlobalISel
							LLVMAsmPrinter
							LLVMDebugInfoDWARF
							LLVMSelectionDAG
							LLVMCodeGen
							LLVMScalarOpts
							LLVMAggressiveInstCombine
							LLVMInstCombine
							LLVMBitWriter
							LLVMTarget
							LLVMTransformUtils
							LLVMAnalysis
							LLVMProfileData
							LLVMObject
							LLVMBitReader
							LLVMRemarks
							LLVMBitstreamReader
							LLVMMCParser
							LLVMMCDisassembler
							LLVMMC
							LLVMBinaryFormat
							LLVMDebugInfoCodeView
							LLVMDebugInfoMSF
							z
							$<$<PLATFORM_ID:Linux>:dl>
							Threads::Threads
							LLVMDemangle
)

add_subdirectory(Tests)