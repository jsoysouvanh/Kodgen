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
#include "Kodgen/Misc/TomlUtility.h"
#include "Kodgen/CodeGen/FileGenerationSettings.h"
#include "Kodgen/CodeGen/FileGenerationResult.h"
#include "Kodgen/CodeGen/FileGenerationUnit.h"
#include "Kodgen/Parsing/FileParserFactory.h"
#include "Kodgen/Properties/NativeProperties/ParseAllNestedPropertyRule.h"
#include "Kodgen/Threading/ThreadPool.h"
#include "Kodgen/Threading/TaskHelper.h"

namespace kodgen
{
	class FileGenerator
	{
		private:
			/** Section name used in the toml file for FileGenerator settings. */
			static constexpr char const*	_tomlSettingsSectionName	= "FileGenerationSettings";

			/** Native property rules. */
			ParseAllNestedPropertyRule		_parseAllNestedPropertyRule;

			/**
			*	@brief Process all provided files on multiple threads.
			*	
			*	@param fileParserFactory	Factory to use to generate a file parser for each thread.
			*	@param fileGenerationUnit	Generation unit used to generate files. It must have a clean state when this method is called.
			*	@param toProcessFiles		Collection of all files to process.
			*	@param out_genResult		Reference to the generation result to fill during file generation.
			*	@param threadCount			Number of additional threads to use to process the files.
			*/
			template <template <typename> typename FileParserFactoryType, typename FileParserType, typename FileGenerationUnitType>
			void	processFilesMultithread(FileParserFactoryType<FileParserType>&	fileParserFactory,
											FileGenerationUnitType&					fileGenerationUnit,
											std::set<fs::path> const&				toProcessFiles,
											FileGenerationResult&					out_genResult,
											uint32									threadCount)			const	noexcept;

			/**
			*	@brief Process all provided files on the main thread.
			*
			*	@param fileParserFactory	Factory to use to generate the file parser.
			*	@param fileGenerationUnit	Generation unit used to generate files. It must have a clean state when this method is called.
			*	@param toProcessFiles		Collection of all files to process.
			*	@param out_genResult		Reference to the generation result to fill during file generation.
			*/
			template <template <typename> typename FileParserFactoryType, typename FileParserType, typename FileGenerationUnitType>
			void	processFilesMonothread(FileParserFactoryType<FileParserType>&	fileParserFactory,
										   FileGenerationUnitType&					fileGenerationUnit,
										   std::set<fs::path> const&				toProcessFiles,
										   FileGenerationResult&					out_genResult)			const	noexcept;

			/**
			*	@brief Identify all files which will be parsed & regenerated.
			*	
			*	@param fileGenerationUnit	Generation unit used to determine whether a file should be reparsed/regenerated or not.
			*	@param out_genResult		Reference to the generation result to fill during file generation.
			*	@param forceRegenerateAll	Should all files be regenerated or not (regardless of FileGenerator::shouldRegenerateFile() returned value).
			*
			*	@return A collection of all files which will be regenerated.
			*/
			std::set<fs::path>		identifyFilesToProcess(FileGenerationUnit const&	fileGenerationUnit,
														   FileGenerationResult&		out_genResult,
														   bool							forceRegenerateAll)	const	noexcept;

			/**
			*	@brief	Check that the generation output directory is a valid directory.
			*			If the directory doesn't exist, the method will try to create it.
			* 
			*	@param out_genResult Reference to the generation result to fill during file generation.
			* 
			*	@return true if the output directory is a valid directory at the end of the function (the directory was already valid or was created successfully), else false.
			*/
			bool					checkOutputDirectory(FileGenerationResult& out_genResult)				const	noexcept;

			/**
			*	@brief	Get the number of threads to use based on the provided thread count.
			*			If 0 is provided, std::thread::hardware_concurrency is used, or 8 if std::thread::hardware_concurrency returns 0.
			*			For all other initial thread count values, the function returns immediately this number.
			* 
			*	@param initialThreadCount The number of threads to use.
			* 
			*	@return The number of threads to use.
			*/
			uint32					getThreadCount(uint32 initialThreadCount)								const	noexcept;

			/**
			*	@brief Provide information on whether the generated code for @param filePath should be regenerated or not.
			*
			*	@param filePath Path to the file we want to check the status.
			*
			*	@return true if the corresponding generated file should be regenerated, else false.
			*/
			bool					shouldRegenerateFile(fs::path const& filePath)							const	noexcept;
			
			/**
			*	@brief Build the generated file path from a source file.
			*	
			*	@param sourceFilePath	Path to the source file to parse & generate code from.
			*	
			*	@return The path to the generated file.
			*/
			fs::path				makePathToGeneratedFile(fs::path const& sourceFilePath)					const	noexcept;
			
			/**
			*	@brief Add native property rules to the parsing settings.
			*	
			*	@param propParsingSettings Property parsing settings of the file parser.
			*/
			void					addNativePropertyRules(PropertyParsingSettings& propParsingSettings)	const	noexcept;
			
			/**
			*	@brief Remove native property rules from the parsing settings.
			*	
			*	@param propParsingSettings Property parsing settings of the file parser.
			*/
			void					clearNativePropertyRules(PropertyParsingSettings& propParsingSettings)	const	noexcept;

			/**
			*	@brief Generate / update the entity macros file.
			*	
			*	@param fileParserFactory FileParserFactory containing parsing settings.
			*/
			void					generateMacrosFile(FileParserFactoryBase& fileParserFactory)			const	noexcept;

			/**
			*	@brief Prepare the file generation unit for generation, forwarding any required generation settings.
			*	
			*	@param fileGenerationUnit The generationUnit to setup.
			*/
			void					setupFileGenerationUnit(FileGenerationUnit& fileGenerationUnit)			const	noexcept;

			/**
			*	@brief Check that all generation settings are valid.
			*/
			void					checkGenerationSettings()												const	noexcept;

			/**
			*	@brief Load the generatedFilesExtension setting from toml.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadGeneratedFilesExtension(toml::value const& tomlGeneratorSettings)			noexcept;

			/**
			*	@brief Load the entityMacrosFilename setting from toml.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadEntityMacrosFilename(toml::value const& tomlGeneratorSettings)				noexcept;

			/**
			*	@brief	Load the supportedExtensions setting from toml.
			*			Loaded extensions completely replace previously supported extensions if any.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadSupportedExtensions(toml::value const& tomlGeneratorSettings)				noexcept;

			/**
			*	@brief Load the outputDirectory setting from toml.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadOutputDirectory(toml::value const& tomlGeneratorSettings)					noexcept;

			/**
			*	@brief	Load the toParseFiles setting from toml.
			*			Loaded files completely replace previous toParseFiles if any.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadToParseFiles(toml::value const& tomlGeneratorSettings)						noexcept;

			/**
			*	@brief	Load the toParseDirectories setting from toml.
			*			Loaded directories completely replace previous toParseDirectories if any.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadToParseDirectories(toml::value const& tomlGeneratorSettings)				noexcept;

			/**
			*	@brief	Load the ignoredFiles setting from toml.
			*			Loaded files completely replace previous ignoredFiles if any.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadIgnoredFiles(toml::value const& tomlGeneratorSettings)						noexcept;

			/**
			*	@brief	Load the ignoredDirectories setting from toml.
			*			Loaded directories completely replace previous ignoredDirectories if any.
			*
			*	@param tomlGeneratorSettings Toml content.
			*/
			void					loadIgnoredDirectories(toml::value const& tomlGeneratorSettings)				noexcept;

		public:
			/** Logger used to issue logs from the FileGenerator. */
			ILogger*				logger	= nullptr;

			/** Struct containing all generation settings. */
			FileGenerationSettings	settings;

			FileGenerator()						= default;
			FileGenerator(FileGenerator const&)	= default;
			FileGenerator(FileGenerator&&)		= default;
			virtual ~FileGenerator()			= default;

			/**
			*	@brief Parse registered files if they were modified since last generation (or don't exist)
			*			and forward them to individual file generation unit for code generation.
			*
			*	@param fileParserFactory	Factory to use to generate the file parser(s).
			*	@param fileGenerationUnit	Generation unit used to generate files. It must have a clean state when this method is called.
			*	@param forceRegenerateAll	Ignore the last write time check and reparse / regenerate all files.
			*	@param threadCount			Number of threads to use for file parsing and generation.
			*								If 0 is provided, the number of concurrent threads supported by the implementation will be used (std::thread::hardware_concurrency(), and 8 if std::thread::hardware_concurrency() returns 0).
			*								If 1 is provided, all the process will be handled by the main thread.
			*
			*	@return Structure containing file generation report.
			*/
			template <template <typename> typename FileParserFactoryType, typename FileParserType, typename FileGenerationUnitType>
			FileGenerationResult generateFiles(FileParserFactoryType<FileParserType>&	fileParserFactory,
											   FileGenerationUnitType&					fileGenerationUnit,
											   bool										forceRegenerateAll	= false,
											   uint32									threadCount			= 0)	noexcept;

			/**
			*	@brief Add a new template to the list of generated code templates.
			*		   if @param templateName is already defined in the generator, replace it.
			*
			*	@param templateName Name of the code template which will be specified in the source code.
			*	@param codeTemplate Pointer to a GeneratedCodeTemplate instance.
			*						The provided instance has be deleted manually by the user if newed.
			*/
			void addGeneratedCodeTemplate(std::string const&		templateName,
										  GeneratedCodeTemplate*	codeTemplate)	noexcept;

			/**
			*	@brief Set the default generated code template to use with the specified entity type when no template is specified in the entity properties.
			*
			*	@brief entityType	Type of the entity we set the default generated code template for.
			*						It can only be one of the following: Namespace, Class, Struct, Enum.
			*	@brief template		Name of the default generated code template.
			*						It must have been setup using the addGeneratedCodeTemplate(...) method before.
			*
			*	@return true if the new default generated code template was setup successfully, else false.
			*/
			bool setDefaultGeneratedCodeTemplate(EEntityType		entityType,
												 std::string const&	templateName)	noexcept;

			/**
			*	@brief Setup this object's parameters with the provided toml file. Unset settings remain unchanged.
			*
			*	@param pathToSettingsFile Path to the toml file.
			*
			*	@return true if a file could be loaded, else false.
			*/
			bool loadSettings(fs::path const& pathToSettingsFile)					noexcept;
	};

	#include "Kodgen/CodeGen/FileGenerator.inl"
}