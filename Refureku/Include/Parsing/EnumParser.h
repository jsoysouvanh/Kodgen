#pragma once

#include <clang-c/Index.h>

#include "FundamentalTypes.h"
#include "InfoStructures/ParsingInfo.h"

namespace refureku
{
	class EnumParser
	{
		private:
			bool		_shouldCheckValidity	= false;
			bool		_isCurrentlyParsing		= false;
			CXCursor	_currentCursor;

			bool	isEnumValid(CXCursor currentCursor)	noexcept;
			void	endParsing()						noexcept;

		public:
			EnumParser()					= default;
			EnumParser(EnumParser const&)	= default;
			EnumParser(EnumParser&&)		= default;
			~EnumParser()					= default;

			CXChildVisitResult	parse(CXCursor currentCursor, ParsingInfo& parsingInfo)					noexcept;
			void				updateParsingState(CXCursor parent)										noexcept;
			void				startParsing(CXCursor currentCursor, ParsingInfo& parsingInfo)			noexcept;
			
			bool				isCurrentlyParsing()											const	noexcept;
	};
}