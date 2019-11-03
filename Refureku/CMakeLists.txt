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
	#target_link_libraries(refureku PRIVATE $<IF:$<PLATFORM_ID:Linux>,,libclang.lib>)

	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)

	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#		target_link_libraries(refureku PRIVATE $<$<CXX_COMPILER_ID:Clang>:c++abi>)
		target_link_libraries(refureku PRIVATE
								$<$<CXX_COMPILER_ID:Clang>:c++>
								$<$<CXX_COMPILER_ID:Clang>:c++abi>
#								$<$<CXX_COMPILER_ID:Clang>:c>
#								$<$<CXX_COMPILER_ID:Clang>:gcc_s>
#								$<$<CXX_COMPILER_ID:Clang>:gcc>
								libclang.a
								libclangAST.a
								libclangBasic.a
								libclangDriver.a
								libclangFrontend.a
								libclangIndex.a
								libclangLex.a
								libclangSema.a
								libclangSerialization.a
								libclangTooling.a
								libclangARCMigrate.a
								libLLVMAArch64CodeGen.a
								libLLVMAArch64AsmParser.a
								libLLVMAArch64Desc.a
								libLLVMAArch64Disassembler.a
								libLLVMAArch64Info.a
								libLLVMAArch64Utils.a
								libLLVMAMDGPUCodeGen.a
								libLLVMAMDGPUAsmParser.a
								libLLVMAMDGPUDesc.a
								libLLVMAMDGPUDisassembler.a
								libLLVMAMDGPUInfo.a
								libLLVMAMDGPUUtils.a
								libLLVMARMCodeGen.a
								libLLVMARMAsmParser.a
								libLLVMARMDesc.a
								libLLVMARMDisassembler.a
								libLLVMARMInfo.a
								libLLVMARMUtils.a
								libLLVMBPFCodeGen.a
								libLLVMBPFAsmParser.a
								libLLVMBPFDesc.a
								libLLVMBPFDisassembler.a
								libLLVMBPFInfo.a
								libLLVMHexagonCodeGen.a
								libLLVMHexagonAsmParser.a
								libLLVMHexagonDesc.a
								libLLVMHexagonDisassembler.a
								libLLVMHexagonInfo.a
								libLLVMLanaiCodeGen.a
								libLLVMLanaiAsmParser.a
								libLLVMLanaiDesc.a
								libLLVMLanaiDisassembler.a
								libLLVMLanaiInfo.a
								libLLVMMipsCodeGen.a
								libLLVMMipsAsmParser.a
								libLLVMMipsDesc.a
								libLLVMMipsDisassembler.a
								libLLVMMipsInfo.a
								libLLVMMSP430CodeGen.a
								libLLVMMSP430AsmParser.a
								libLLVMMSP430Desc.a
								libLLVMMSP430Disassembler.a
								libLLVMMSP430Info.a
								libLLVMNVPTXCodeGen.a
								libLLVMNVPTXDesc.a
								libLLVMNVPTXInfo.a
								libLLVMPowerPCCodeGen.a
								libLLVMPowerPCAsmParser.a
								libLLVMPowerPCDesc.a
								libLLVMPowerPCDisassembler.a
								libLLVMPowerPCInfo.a
								libLLVMRISCVCodeGen.a
								libLLVMRISCVAsmParser.a
								libLLVMRISCVDesc.a
								libLLVMRISCVDisassembler.a
								libLLVMRISCVInfo.a
								libLLVMRISCVUtils.a
								libLLVMSparcCodeGen.a
								libLLVMSparcAsmParser.a
								libLLVMSparcDesc.a
								libLLVMSparcDisassembler.a
								libLLVMSparcInfo.a
								libLLVMSystemZCodeGen.a
								libLLVMSystemZAsmParser.a
								libLLVMSystemZDesc.a
								libLLVMSystemZDisassembler.a
								libLLVMSystemZInfo.a
								libLLVMWebAssemblyCodeGen.a
								libLLVMWebAssemblyAsmParser.a
								libLLVMWebAssemblyDesc.a
								libLLVMWebAssemblyDisassembler.a
								libLLVMWebAssemblyInfo.a
								libLLVMX86CodeGen.a
								libLLVMX86AsmParser.a
								libLLVMX86Desc.a
								libLLVMX86Disassembler.a
								libLLVMX86Info.a
								libLLVMX86Utils.a
								libLLVMXCoreCodeGen.a
								libLLVMXCoreDesc.a
								libLLVMXCoreDisassembler.a
								libLLVMXCoreInfo.a
								libLLVMCore.a
								libLLVMSupport.a
								libclangFormat.a
								libclangToolingInclusions.a
								libclangToolingCore.a
								libclangFrontend.a
								libclangDriver.a
								libLLVMOption.a
								libclangParse.a
								libclangSerialization.a
								libclangSema.a
								libclangEdit.a
								libclangRewrite.a
								libclangAnalysis.a
								libclangASTMatchers.a
								libclangAST.a
								libclangLex.a
								libclangBasic.a
								libLLVMAArch64Desc.a
								libLLVMAArch64Info.a
								libLLVMAArch64Utils.a
								libLLVMMIRParser.a
								libLLVMAMDGPUDesc.a
								libLLVMAMDGPUInfo.a
								libLLVMAMDGPUUtils.a
								libLLVMARMDesc.a
								libLLVMARMInfo.a
								libLLVMARMUtils.a
								libLLVMHexagonDesc.a
								libLLVMHexagonInfo.a
								libLLVMLanaiDesc.a
								libLLVMLanaiInfo.a
								libLLVMipo.a
								libLLVMVectorize.a
								libLLVMIRReader.a
								libLLVMAsmParser.a
								libLLVMInstrumentation.a
								libLLVMLinker.a
								libLLVMSystemZDesc.a
								libLLVMSystemZInfo.a
								libLLVMWebAssemblyDesc.a
								libLLVMWebAssemblyInfo.a
								libLLVMGlobalISel.a
								libLLVMAsmPrinter.a
								libLLVMDebugInfoDWARF.a
								libLLVMSelectionDAG.a
								libLLVMCodeGen.a
								libLLVMScalarOpts.a
								libLLVMAggressiveInstCombine.a
								libLLVMInstCombine.a
								libLLVMBitWriter.a
								libLLVMTarget.a
								libLLVMTransformUtils.a
								libLLVMAnalysis.a
								libLLVMProfileData.a
								libLLVMObject.a
								libLLVMBitReader.a
								libLLVMCore.a
								libLLVMRemarks.a
								libLLVMBitstreamReader.a
								libLLVMMCParser.a
								libLLVMMCDisassembler.a
								libLLVMMC.a
								libLLVMBinaryFormat.a
								libLLVMDebugInfoCodeView.a
								libLLVMDebugInfoMSF.a
								libLLVMSupport.a
								z
								rt
								dl
								Threads::Threads
								m
								libLLVMDemangle.a
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
