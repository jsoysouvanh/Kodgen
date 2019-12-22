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
			std::optional<ParsingResult>	_parsingResult	= std::nullopt;
			
			ClassParser						_classParser;
			EnumParser						_enumParser;
			ParsingInfo						_parsingInfo;

			static CXChildVisitResult	staticParseCursor(CXCursor c, CXCursor parent, CXClientData clientData)		noexcept;
			
			void						updateParsingState(CXCursor parent)											noexcept;
			CXChildVisitResult			parseCursor(CXCursor currentCursor, CXCursor parentCursor)					noexcept;

			CXChildVisitResult			parseDefault(CXCursor currentCursor)										noexcept;

		public:
			ParsingSettings	parsingSettings;

			Parser()				noexcept;
			Parser(Parser const&)	= default;
			Parser(Parser&&)		= default;
			~Parser()				noexcept;

			virtual bool			parse(fs::path const& parseFile)		noexcept;

			/**
			*	Get the parsing result of the previous parsing operation if available, else nullptr
			*/
			ParsingResult const*	retrieveParsingResult()			const	noexcept;

			/**
			*	Remove all previously parsed information from the class
			*/
			void					clear()									noexcept;
	};
}