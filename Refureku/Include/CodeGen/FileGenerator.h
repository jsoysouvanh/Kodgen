#pragma once

#include <set>
#include <unordered_map>
#include <string>
#include <type_traits>

#include "Misc/SafeFilesystem.h"
#include "CodeGen/GeneratedCodeTemplate.h"
#include "Parsing/Parser.h"

namespace refureku
{
	class FileGenerator
	{
		private:
			std::set<fs::path>										_includedFiles;
			std::set<fs::path>										_includedDirectories;
			std::unordered_map<std::string,	GeneratedCodeTemplate*>	_generatedCodeTemplates;
			GeneratedCodeTemplate*									_defaultCodeTemplate = nullptr;

		protected:

		public:
			FileGenerator()		noexcept;
			~FileGenerator()	noexcept;

			/**
			*	@brief Add a file to the list of files to parse.
			*
			*	@return true on success (the path exists and is a file), else false
			*/

			bool addFile(fs::path filePath)													noexcept;

			/**
			*	@brief Add a directory to the list of directories to parse.
			*	@brief All directories contained in the given directory will be recursively inspected.
			*	@brief All files contained in any included directory will be parsed.
			*
			*	@return true on success (the path exists and is a directory), else false
			*/
			bool addDirectory(fs::path dirPath)												noexcept;

			/**
			*	@brief Add a new template to the list of generated code templates
			*	@brief This will override any template having the same templateName
			*
			*	@param templateName Name of the code template which will be specified in the source code
			*	@param isDefault Should this template be used when no template is specified in source code
			*/
			template <typename T>
			void addGeneratedCodeTemplate(std::string templateName, bool isDefault = false)	noexcept;

			/**
			*	@brief Generate all registered files using
			*/
			bool generateFiles(Parser& parser, bool forceRegenerateAll = false)				noexcept;
	};

	#include "CodeGen/FileGenerator.inl"
}