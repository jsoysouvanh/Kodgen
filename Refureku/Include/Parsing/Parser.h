#pragma once

#include <clang-c/Index.h>

#include "Misc/Filesystem.h"
#include "Misc/Optional.h"
#include "InfoStructures/ParsingInfo.h"
#include "Parsing/ClassParser.h"
#include "Parsing/EnumParser.h"
#include "Parsing/ParsingResult.h"
#include "Parsing/ParsingSettings.h"
#include "Properties/PropertyParser.h"

namespace refureku
{
	class Parser
	{
		private:
			static constexpr char const*	_parseArguments[] = { "-x", "c++", "-D", "REFUREKU_PARSING" };
			
			CXIndex							_clangIndex;
			
			ClassParser						_classParser;
			EnumParser						_enumParser;
			ParsingInfo						_parsingInfo;

			static CXChildVisitResult	staticParseCursor(CXCursor c, CXCursor parent, CXClientData clientData)		noexcept;
			
			void						setupForParsing()															noexcept;

			void						updateParsingState(CXCursor parent)											noexcept;
			CXChildVisitResult			parseCursor(CXCursor currentCursor)											noexcept;

			CXChildVisitResult			parseDefault(CXCursor currentCursor)										noexcept;

		protected:
			/**
			*	@brief Overridable method called just before starting the parsing process of a file
			*
			*	@param parseFile Path to the file which is about to be parsed
			*/
			virtual void preParse(fs::path const& parseFile)								noexcept;

			/**
			*	@brief Overridable method called just after the parsing process has been finished
			*	@brief Even if the parsing process ended prematurely, this method is called
			*
			*	@param parseFile Path to the file which has been parsed
			*	@param result Result of the parsing
			*/
			virtual void postParse(fs::path const& parseFile, ParsingResult const& result)	noexcept;

		public:
			ParsingSettings	parsingSettings;

			Parser()				noexcept;
			Parser(Parser const&)	= default;
			Parser(Parser&&)		= default;
			~Parser()				noexcept;

			bool	parse(fs::path const& parseFile, ParsingResult& out_result)		noexcept;

			/**
			*	Remove all previously parsed information from the class
			*/
			void	clear()															noexcept;
	};
}