cmake_minimum_required(VERSION 3.8)

project(Refureku)

set(IS_SUPPORTED_COMPILER NO)
set(IS_CLANG_OR_GCC NO)

# Handle specific flags for each supported compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	
	add_compile_options(-stdlib=libc++)
	#link_libraries(-stdlib=libc++)
	
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
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Binaries)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Binaries)

	# Setup warning level
	if (MSVC)
		add_compile_options(/W4)
	else()	# Clang or GCC
		add_compile_options(-Wall -Wextra -Wpedantic)
	endif()

	# Setup executable / lib
	add_executable(refureku Source/main.cpp)

	# Link to required libraries
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)

	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		target_link_libraries(refureku PRIVATE
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
								LLVMAArch64Desc
								LLVMAArch64Info
								LLVMAArch64Utils
								LLVMMIRParser
								LLVMAMDGPUDesc
								LLVMAMDGPUInfo
								LLVMAMDGPUUtils
								LLVMARMDesc
								LLVMARMInfo
								LLVMARMUtils
								LLVMHexagonDesc
								LLVMHexagonInfo
								LLVMLanaiDesc
								LLVMLanaiInfo
								LLVMipo
								LLVMVectorize
								LLVMIRReader
								LLVMAsmParser
								LLVMInstrumentation
								LLVMLinker
								LLVMSystemZDesc
								LLVMSystemZInfo
								LLVMWebAssemblyDesc
								LLVMWebAssemblyInfo
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
								LLVMCore
								LLVMRemarks
								LLVMBitstreamReader
								LLVMMCParser
								LLVMMCDisassembler
								LLVMMC
								LLVMBinaryFormat
								LLVMDebugInfoCodeView
								LLVMDebugInfoMSF
								LLVMSupport
								z
								rt
								dl
								Threads::Threads
								m
								LLVMDemangle
		)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
		target_link_libraries(refureku PRIVATE 
								libclang.lib
		)
	endif()


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
