/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <set>
#include <cassert>
#include <type_traits>	//std::is_base_of
#include <chrono>

#include "Kodgen/Misc/ILogger.h"
#include "Kodgen/Misc/Settings.h"
#include "Kodgen/CodeGen/FileGenerationResult.h"
#include "Kodgen/CodeGen/CodeGenUnit.h"
#include "Kodgen/Parsing/FileParser.h"
#include "Kodgen/Threading/ThreadPool.h"
#include "Kodgen/Threading/TaskHelper.h"

namespace kodgen
{
	//Forward declaration
	class FileGeneratorSettings;

	class FileGenerator
	{
		private:
			/**
			*	@brief Process all provided files on multiple threads.
			*	
			*	@param fileParser		Original file parser to use to parse registered files. A copy of this parser will be used for each generation thread.
			*	@param codeGenUnit		Generation unit used to generate files. It must have a clean state when this method is called.
			*	@param toProcessFiles	Collection of all files to process.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*	@param threadCount		Number of additional threads to use to process the files.
			*/
			template <typename FileParserType, typename CodeGenUnitType, typename CodeGenEnvType>
			void	processFilesMultithread(FileParserType&				fileParser,
											CodeGenUnitType&			codeGenUnit,
											CodeGenEnvType&				env,
											std::set<fs::path> const&	toProcessFiles,
											FileGenerationResult&		out_genResult,
											uint32						threadCount)					const	noexcept;

			/**
			*	@brief Process all provided files on the main thread.
			*
			*	@param fileParser		File parser to use to parse registered files.
			*	@param codeGenUnit		Generation unit used to generate files. It must have a clean state when this method is called.
			*	@param toProcessFiles	Collection of all files to process.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			template <typename FileParserType, typename CodeGenUnitType, typename CodeGenEnvType>
			void	processFilesMonothread(FileParserType&				fileParser,
										   CodeGenUnitType&				codeGenUnit,
										   CodeGenEnvType&				env,
										   std::set<fs::path> const&	toProcessFiles,
										   FileGenerationResult&		out_genResult)					const	noexcept;

			/**
			*	@brief Identify all files which will be parsed & regenerated.
			*	
			*	@param codeGenUnit			Generation unit used to determine whether a file should be reparsed/regenerated or not.
			*	@param out_genResult		Reference to the generation result to fill during file generation.
			*	@param forceRegenerateAll	Should all files be regenerated or not (regardless of FileGenerator::shouldRegenerateFile() returned value).
			*
			*	@return A collection of all files which will be regenerated.
			*/
			std::set<fs::path>		identifyFilesToProcess(CodeGenUnit const&		codeGenUnit,
														   FileGenerationResult&	out_genResult,
														   bool						forceRegenerateAll)	const	noexcept;

			/**
			*	@brief	Get the number of threads to use based on the provided thread count.
			*			If 0 is provided, std::thread::hardware_concurrency is used, or 8 if std::thread::hardware_concurrency returns 0.
			*			For all other initial thread count values, the function returns immediately this number.
			* 
			*	@param initialThreadCount The number of threads to use.
			* 
			*	@return The number of threads to use.
			*/
			uint32					getThreadCount(uint32 initialThreadCount)							const	noexcept;

			/**
			*	@brief Generate / update the entity macros file.
			*	
			*	@param parsingSettings	Parsing settings.
			*	@param outputDirectory	Directory in which the macro file should be generated.
			*/
			void					generateMacrosFile(ParsingSettings const&	parsingSettings,
													   fs::path const&			outputDirectory)		const	noexcept;

			/**
			*	@brief Check that everything is setup correctly for generation.
			* 
			*	@param fileParser	The file parser to using during the generation process.
			*	@param codeGenUnit	The code generation to use during the generation process.
			* 
			*	@return true if all settings are correct, else false.
			*/
			bool					checkGenerationSetup(FileParser const&	fileParser,
														 CodeGenUnit const& codeGenUnit)				const	noexcept;

		public:
			/** Logger used to issue logs from the FileGenerator. */
			ILogger*						logger		= nullptr;

			/** Struct containing all generation settings. */
			FileGeneratorSettings const*	settings	= nullptr;

			/**
			*	@brief	Parse registered files if they were modified since last generation (or don't exist)
			*			and forward them to individual file generation unit for code generation.
			*
			*	@param fileParser			Original file parser to use to parse registered files. A copy of this parser will be used for each generation thread.
			*	@param codeGenUnit			Generation unit used to generate code. It must have a clean state when this method is called.
			*	@param env					Generation environment.
			*	@param forceRegenerateAll	Ignore the last write time check and reparse / regenerate all files.
			*	@param threadCount			Number of threads to use for file parsing and generation.
			*								If 0 is provided, the number of concurrent threads supported by the implementation will be used (std::thread::hardware_concurrency(), and 8 if std::thread::hardware_concurrency() returns 0).
			*								If 1 is provided, all the process will be handled by the main thread.
			*
			*	@return Structure containing file generation report.
			*/
			template <typename FileParserType, typename CodeGenUnitType, typename CodeGenEnvType>
			FileGenerationResult generateFiles(FileParserType&	fileParser,
											   CodeGenUnitType&	codeGenUnit,
											   CodeGenEnvType&	env,
											   bool				forceRegenerateAll	= false,
											   uint32			threadCount			= 0)	noexcept;
	};

	#include "Kodgen/CodeGen/FileGenerator.inl"
}