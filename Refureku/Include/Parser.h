#pragma once

#include <clang-c/Index.h>
#include <optional>

#include "SafeFilesystem.h"
#include "ParsingSettings.h"
#include "ParsingResult.h"
#include "Properties/PropertyParser.h"

namespace refureku
{
	class ParsingInfo;	//Forward declaration

	class Parser
	{
		private:
			static constexpr char const*	_parseArguments[] = { "-x", "c++", "-D", "REFUREKU_PARSING" };
			CXIndex							_clangIndex;
			std::optional<ParsingResult>	_parsingResult	= std::nullopt;

			static CXChildVisitResult	staticParseCursor(CXCursor c, CXCursor parent, CXClientData clientData)					noexcept;
			static void					updateParsingState(CXCursor parent, ParsingInfo* parsingInfo)							noexcept;
			static CXChildVisitResult	parseCursor(CXCursor currentCursor, CXCursor parentCursor, ParsingInfo* parsingInfo)	noexcept;

			static CXChildVisitResult	parseDefault(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;

			static CXChildVisitResult	parseClassContent(CXCursor currentCursor, ParsingInfo* parsingInfo)						noexcept;

			static CXChildVisitResult	parseEnumContent(CXCursor currentCursor, ParsingInfo* parsingInfo)						noexcept;
			static bool					isEnumValid(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;

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
	};
}