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
			std::ofstream*	_writingFile = nullptr;

			/**
			*	@brief Write a single line in the generated file
			*	@brief This method is the same as writeLine(std::string const& line) but is here to end the variadic writeLines(...) recurrency
			*/
			void writeLines(std::string const& line)								noexcept;
			void writeLines(std::string&& line)										noexcept;

		protected:
			/**
			*	@brief Write a line in the generated file
			*
			*	@param line Line to write in the generated file
			*/
			void writeLine(std::string const& line)									noexcept;
			void writeLine(std::string&& line)										noexcept;

			/**
			*	@brief Write multiple lines in the generated file
			*
			*	@param line First line to write
			*	@param... otherLines Other lines to write
			*/
			template <typename... Lines>
			void writeLines(std::string const& line, Lines&&... otherLines)			noexcept;

			template <typename... Lines>
			void writeLines(std::string&& line, Lines&&... otherLines)				noexcept;

			/**
			*	@brief Write a macro in the generated file
			*	@brief Each argument represent a new line. \ are inserted automatically at the end of each line
			*
			*	@param macroPrototype Full prototype of the macro, for example: "MY_MACRO(arg1, arg2)" or "MY_MACRO2(...)"
			*	@param... lines Body of the macro. Can be composed of several lines for a better visibility.
			*/
			template <typename... Lines>
			void writeMacro(std::string&& macroPrototype, Lines&&... lines)			noexcept;

			/**
			*	@brief Write multiple lines in the generated file, each line starting with a \t (one indentation level)
			*	@brief and ending with a \
			*
			*	@param line First line to write
			*	@param lines... Some other lines
			*/
			template <typename... Lines>
			void expandWriteMacroLines(std::string&& line, Lines&&... otherLines)	noexcept;

			/**
			*	@brief Write a single line starting with a \t (one indentation level) and ending with a \ in the generated file
			*
			*	@param line Line to write in the generated file
			*/
			void expandWriteMacroLines(std::string&& line)							noexcept;

		public:
			GeneratedCodeTemplate()								= default;
			GeneratedCodeTemplate(GeneratedCodeTemplate const&) = delete;
			GeneratedCodeTemplate(GeneratedCodeTemplate&&)		= delete;
			virtual ~GeneratedCodeTemplate()					= default;

			/**
			*	@brief Write lines into the generated file
			*
			*	@param parsedFile Path to the parsed file
			*	@param entityInfo Structure containing info about the entity. You may static cast this into the derived class you expect
			*/
			virtual void generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo)	noexcept = 0;

			/**
			*	Setup the stream to use for writing
			*/
			void setWritingStream(std::ofstream* writingFile)									noexcept;
	};

	#include "CodeGen/GeneratedCodeTemplate.inl"
}