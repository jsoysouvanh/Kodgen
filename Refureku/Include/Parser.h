#pragma once

#include <clang-c/Index.h>

#include "SafeFilesystem.h"

namespace refureku
{
	class ParsingInfo;	//Forward declaration

	class Parser
	{
		private:
			static char const* _parseArguments[];

			static CXChildVisitResult	staticParseCursor(CXCursor c, CXCursor parent, CXClientData clientData);
			static void					updateParsingState(CXCursor parent, ParsingInfo* parsingInfo);
			static CXChildVisitResult	parseCursor(CXCursor currentCursor, CXCursor parentCursor, ParsingInfo* parsingInfo);

			static CXChildVisitResult	parseDefault(CXCursor currentCursor, ParsingInfo* parsingInfo);

			static CXChildVisitResult	parseClassContent(CXCursor currentCursor, ParsingInfo* parsingInfo);
			static bool					isClassValid(CXCursor currentCursor, ParsingInfo* parsingInfo);
			static CXChildVisitResult	parseField(CXCursor currentCursor, ParsingInfo* parsingInfo);
			static bool					isFieldValid(CXCursor currentCursor, ParsingInfo* parsingInfo);
			static CXChildVisitResult	parseMethod(CXCursor currentCursor, ParsingInfo* parsingInfo);
			static bool					isMethodValid(CXCursor currentCursor, ParsingInfo* parsingInfo);

			static CXChildVisitResult	parseEnumContent(CXCursor currentCursor, ParsingInfo* parsingInfo);
			static bool					isEnumValid(CXCursor currentCursor, ParsingInfo* parsingInfo);

		protected:
			CXIndex _clangIndex;

			static	std::string			getString(CXString&& clangString);

		public:
			Parser() noexcept;
			~Parser() noexcept;

			virtual bool parse(fs::path const& parseFile) const noexcept;
	};
}