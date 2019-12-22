#pragma once

#include <clang-c/Index.h>

#include "FundamentalTypes.h"
#include "InfoStructures/ParsingInfo.h"

namespace refureku
{
	class EnumParser
	{
		private:
			bool	_shouldCheckValidity	= false;
			bool	_isParsing				= false;

			bool	isEnumValid(CXCursor currentCursor)	noexcept;

		public:
			EnumParser()					= default;
			EnumParser(EnumParser const&)	= default;
			EnumParser(EnumParser&&)		= default;
			~EnumParser()					= default;

			CXChildVisitResult parse(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept;
	};
}