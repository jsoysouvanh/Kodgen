#pragma once

#include <set>
#include <unordered_map>
#include <string>
#include <type_traits>

#include "Misc/Filesystem.h"
#include "CodeGen/GeneratedCodeTemplate.h"
#include "CodeGen/FileGenerationResult.h"
#include "Parsing/Parser.h"

namespace refureku
{
	class FileGenerator
	{
		private:
			std::set<fs::path>										_includedFiles;
			std::set<fs::path>										_includedDirectories;
			std::unordered_map<std::string,	GeneratedCodeTemplate*>	_generatedCodeTemplates;
			GeneratedCodeTemplate*									_defaultClassTemplate		= nullptr;
			GeneratedCodeTemplate*									_defaultEnumTemplate		= nullptr;
			std::string												_supportedCodeTemplateRegex = "";

			void					updateSupportedCodeTemplateRegex()																																			noexcept;
			void					generateEntityFile(FileGenerationResult& genResult, fs::path const& filePath, ParsingResult const& parsingResult)															noexcept;
			GeneratedCodeTemplate*	getEntityGeneratedCodeTemplate(EntityInfo& entityInfo, bool isClass, EFileGenerationError& out_error)																const	noexcept;
			void					writeEntityToFile(EntityInfo& entityInfo, fs::path const& filePath, std::ofstream* stream, FileGenerationResult& genResult, bool isClass)									noexcept;
			bool					shouldRegenerateFile(fs::path const& filePath)																														const	noexcept;
			fs::path				makePathToGeneratedFile(fs::path const& sourceFilePath)																												const	noexcept;

		public:
			/**
			*	@brief Main (complex) property name used to specify code generator in source code
			*/
			std::string	codeTemplateMainComplexPropertyName = "GenTemplate";

			/**
			*	@brief Extension used for generated files
			*/
			std::string generatedFilesExtension = ".refureku.h";

			/**
			*	@brief Path to the folder all files should be generated (and where existing ones are)
			*/
			fs::path pathToGeneratedFilesFolder;

			FileGenerator()		noexcept;
			~FileGenerator()	noexcept;

			/**
			*	@brief Add a file to the list of files to parse.
			*
			*	@return true on success (the path exists and is a file), else false
			*/

			bool addFile(fs::path filePath)																	noexcept;

			/**
			*	@brief Add a directory to the list of directories to parse.
			*	@brief All directories contained in the given directory will be recursively inspected.
			*	@brief All files contained in any included directory will be parsed.
			*
			*	@return true on success (the path exists and is a directory), else false
			*/
			bool addDirectory(fs::path dirPath)																noexcept;

			/**
			*	@brief Add a new template to the list of generated code templates
			*	@brief This will override any template having the same templateName
			*
			*	@param templateName Name of the code template which will be specified in the source code
			*	@param codeTemplate Pointer to a GeneratedCodeTemplate instance (must be newed).
			*			The instance will be deleted by the FileGenerator when destroyed
			*/
			void addGeneratedCodeTemplate(std::string templateName, GeneratedCodeTemplate* codeTemplate)	noexcept;

			/**
			*	@brief Setup the generated code template to use when no template is specified in a parsed source code class
			*
			*	@param codeTemplate The name of the default generated code template.
			*						It must have been setup using the addGeneratedCodeTemplate(...) method before
			*
			*	@return true if the new default class template was setup successfully, else false
			*/
			bool setDefaultClassTemplate(std::string templateName)											noexcept;

			/**
			*	@brief Setup the generated code template to use when no template is specified in a parsed source code enum
			*
			*	@param codeTemplate The name of the default generated code template.
			*						It must have been setup using the addGeneratedCodeTemplate(...) method before
			*
			*	@return true if the new default enum template was setup successfully, else false
			*/
			bool setDefaultEnumTemplate(std::string templateName)											noexcept;

			/**
			*	@brief Parse registered files if they were changes since last generation
			*			and try to generate corresponding files using code templates
			*
			*	@param parser The parser to use for the parsing
			*	@param forceRegenerateAll Ignore the last write time check and reparse / regenerate all files
			*
			*	@return Structure containing result report
			*/
			FileGenerationResult generateFiles(Parser& parser, bool forceRegenerateAll = false)				noexcept;
	};
}