#pragma once

#include <clang-c/Index.h>

#include "SafeFilesystem.h"
#include "Properties/PropertyParser.h"

namespace refureku
{
	class ParsingInfo;	//Forward declaration

	class Parser
	{
		private:
			static constexpr char const*	_parseArguments[] = { "-x", "c++", "-D", "REFUREKU_PARSING" };
			CXIndex							_clangIndex;

			static	std::string			getString(CXString&& clangString) noexcept;

			static CXChildVisitResult	staticParseCursor(CXCursor c, CXCursor parent, CXClientData clientData)					noexcept;
			static void					updateParsingState(CXCursor parent, ParsingInfo* parsingInfo)							noexcept;
			static CXChildVisitResult	parseCursor(CXCursor currentCursor, CXCursor parentCursor, ParsingInfo* parsingInfo)	noexcept;

			static CXChildVisitResult	parseDefault(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;

			static CXChildVisitResult	parseClassContent(CXCursor currentCursor, ParsingInfo* parsingInfo)						noexcept;
			static bool					isClassValid(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;
			static CXChildVisitResult	parseField(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;
			static bool					isFieldValid(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;
			static CXChildVisitResult	parseMethod(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;
			static bool					isMethodValid(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;

			static CXChildVisitResult	parseEnumContent(CXCursor currentCursor, ParsingInfo* parsingInfo)						noexcept;
			static bool					isEnumValid(CXCursor currentCursor, ParsingInfo* parsingInfo)							noexcept;

		public:
			PropertyParser propertyParser;

			Parser()				noexcept;
			Parser(Parser const&)	= default;
			Parser(Parser&&)		= default;
			~Parser()				noexcept;

			virtual bool parse(fs::path const& parseFile) noexcept;
	};
}