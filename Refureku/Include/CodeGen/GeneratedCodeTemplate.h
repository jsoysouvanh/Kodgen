#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Misc/SafeFilesystem.h"
#include "InfoStructures/EntityInfo.h"

namespace refureku
{
	class GeneratedCodeTemplate
	{
		private:
			std::ofstream	_writingFile;

		protected:
			void write(std::string&& text) noexcept;

		public:
			GeneratedCodeTemplate()								= default;
			GeneratedCodeTemplate(GeneratedCodeTemplate const&) = delete;
			GeneratedCodeTemplate(GeneratedCodeTemplate&&)		= delete;

			/**
			*	@brief Get the name of the file to generate
			*	
			*	@param parsedFile Path to the parsed file
			*	@param parsingResult Structure containing info about the parsed file
			*
			*	@return Path to the file to generate
			*/
			virtual fs::path getGeneratedFilename(fs::path const& parsedFile, EntityInfo const& entityInfo)	noexcept = 0;

			/**
			*	@brief Write lines into the generated file
			*
			*	@param parsedFile Path to the parsed file
			*	@param entityInfo Structure containing info about the entity. You may cast this into the derived class you expect
			*/
			virtual void generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo)				noexcept;

			/**
			*	@brief Close the stream to the file if any is open
			*/
			void releaseGeneratedFile()																		noexcept;
	};
}