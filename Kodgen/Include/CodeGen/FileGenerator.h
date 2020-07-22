#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <type_traits>

#include "Misc/Filesystem.h"
#include "Misc/ILogger.h"
#include "CodeGen/GeneratedCodeTemplate.h"
#include "CodeGen/FileGenerationResult.h"
#include "CodeGen/GeneratedFile.h"
#include "Parsing/FileParser.h"

namespace kodgen
{
	class FileGenerator
	{
		private:
			/** Name of the internally generated header containing empty definitions for entity macros. */
			static inline std::string										_entityMacrosDefFilename		= "EntityMacros.h";

			/** All generated code templates usable by this generator. */
			std::unordered_map<std::string,	GeneratedCodeTemplate*>			_generatedCodeTemplates;
			
			/** Default generated code templates to use when none is specified in property parameters. */
			std::unordered_map<EntityInfo::EType, GeneratedCodeTemplate*>	_defaultGeneratedCodeTemplates;
			
			/** Regex used internally to determine either a generated code template name is valid or not. */
			std::string														_supportedCodeTemplateRegex		= "";

			void					updateSupportedCodeTemplateRegex()																								noexcept;
			void					generateEntityFile(FileGenerationResult& genResult, fs::path const& filePath, FileParsingResult const& parsingResult)			noexcept;
			GeneratedCodeTemplate*	getEntityGeneratedCodeTemplate(EntityInfo& entityInfo, EFileGenerationError& out_error)									const	noexcept;
			void					writeEntityToFile(GeneratedFile& generatedFile, EntityInfo& entityInfo, FileGenerationResult& genResult)						noexcept;
			bool					shouldRegenerateFile(fs::path const& filePath)																			const	noexcept;
			fs::path				makePathToGeneratedFile(fs::path const& sourceFilePath)																	const	noexcept;
			void					processFile(FileParser2& parser, FileGenerationResult& genResult, fs::path const& pathToFile)									noexcept;
			void					processIncludedFiles(FileParser2& parser, FileGenerationResult& genResult, bool forceRegenerateAll)								noexcept;
			void					processIncludedDirectories(FileParser2& parser, FileGenerationResult& genResult, bool forceRegenerateAll)						noexcept;
			void					refreshPropertyRules(ParsingSettings& parsingSettings)																	const	noexcept;
			void					generateMacrosFile(FileParser2& parser)																					const	noexcept;

		protected:
			/**
			*	@brief Write a header for any generated file
			*
			*	@param file Object which allows to write into the generated file
			*	@param parsingResult Structure containing info about the parsed file
			*/
			virtual void	writeHeader(GeneratedFile& file, FileParsingResult const& parsingResult)	const	noexcept;

			/**
			*	@brief Write a footer for any generated file
			*
			*	@param file Object which allows to write into the generated file
			*	@param parsingResult Structure containing info about the parsed file
			*/
			virtual void	writeFooter(GeneratedFile& file, FileParsingResult const& parsingResult)	const	noexcept;

		public:
			/** Logger used to issue logs from the FileGenerator. */
			ILogger*								logger								= nullptr;

			/** Main (complex) property name used to specify code generator in source code. */
			std::string								codeTemplateMainComplexPropertyName	= "GenTemplate";

			/** Extension used for generated files */
			std::string								generatedFilesExtension				= ".kodgen.h";

			/**
			*	Path to the directory all files should be generated (and where existing ones are).
			*	If the existed directory doesn't exist, it will be created if possible.
			*/
			fs::path								outputDirectory;

			/**
			*	Collection of files to parse.
			*	These files will be parsed without any further check if they exist.
			*	/!\ Make sure all added paths use the os preferred syntax (you should call path.make_preferred() before).
			*/
			std::unordered_set<fs::path, PathHash>	toParseFiles;

			/**
			*	Collection of directories containing files to parse.
			*	All directories contained in the given directories will be recursively inspected, except if they are ignored
			*	All files contained in any parsed directory will be parsed, except if they are ignored or if their extension is not contained in the supportedExtensions.
			*	/!\ Make sure all added paths use the os preferred syntax (you should call path.make_preferred() before)
			*/
			std::unordered_set<fs::path, PathHash>	toParseDirectories;

			/**
			*	Collection of ignored files.
			*	These files will never be parsed (except if they are also part of the includedFiles collection)
			*	/!\ Make sure all added paths use the os preferred syntax (you should call path.make_preferred() before)
			*/
			std::unordered_set<fs::path, PathHash>	ignoredFiles;

			/**
			*	Collection of ignored directories.
			*	All directories contained in the given directories will be ignored, except if they are included
			*	All files contained in any ignored directory will be ignored, except if they are included
			*	/!\ Make sure all added paths use the os preferred syntax (you should call path.make_preferred() before)
			*/
			std::unordered_set<fs::path, PathHash>	ignoredDirectories;

			/** Extensions of files which should be considered for parsing. */
			std::unordered_set<std::string>			supportedExtensions;

			FileGenerator()		noexcept;
			~FileGenerator()	noexcept;

			/**
			*	@brief Add a new template to the list of generated code templates
			*		   This will override any template having the same templateName
			*
			*	@param templateName Name of the code template which will be specified in the source code
			*	@param codeTemplate Pointer to a GeneratedCodeTemplate instance (must be newed).
			*			The instance will be deleted by the FileGenerator when destroyed
			*/
			void addGeneratedCodeTemplate(std::string const& templateName, GeneratedCodeTemplate* codeTemplate)	noexcept;

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
			bool setDefaultGeneratedCodeTemplate(EntityInfo::EType	entityType,
												 std::string const& templateName)							noexcept;

			/**
			*	@brief Parse registered files if they were changes since last generation
			*			and try to generate corresponding files using code templates
			*
			*	@param parser The parser to use for the parsing
			*	@param forceRegenerateAll Ignore the last write time check and reparse / regenerate all files
			*
			*	@return Structure containing result report
			*/
			FileGenerationResult generateFiles(FileParser2& parser, bool forceRegenerateAll = false)		noexcept;

			/**
			*	@brief Setup this object's parameters with the provided toml file. Unset settings remain unchanged.
			*
			*	@param pathToSettingsFile Path to the toml file.
			*
			*	@return true if a file could be loaded, else false.
			*/
			bool				loadSettings(fs::path const& pathToSettingsFile)							noexcept;
	};
}