#pragma once

#include <clang-c/Index.h>
#include <optional>

#include "SafeFilesystem.h"
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
			CXChildVisitResult			parseCursor(CXCursor currentCursor, CXCursor parentCursor)					noexcept;

			CXChildVisitResult			parseDefault(CXCursor currentCursor)										noexcept;

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
			void					clear()									noexcept;
	};
}