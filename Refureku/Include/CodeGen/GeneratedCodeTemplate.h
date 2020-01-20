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
			std::ofstream*	_writingFile;

		protected:
			void write(std::string&& text) noexcept;

		public:
			GeneratedCodeTemplate()								= default;
			GeneratedCodeTemplate(GeneratedCodeTemplate const&) = delete;
			GeneratedCodeTemplate(GeneratedCodeTemplate&&)		= delete;

			/**
			*	@brief Write lines into the generated file
			*
			*	@param parsedFile Path to the parsed file
			*	@param entityInfo Structure containing info about the entity. You may cast this into the derived class you expect
			*/
			virtual void generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo)	noexcept = 0;

			/**
			*	Setup the stream to use for writing
			*/
			void setWritingStream(std::ofstream* writingFile)										noexcept;
	};
}