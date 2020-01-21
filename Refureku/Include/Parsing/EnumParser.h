#pragma once

#include <clang-c/Index.h>

#include "Misc/FundamentalTypes.h"
#include "InfoStructures/ParsingInfo.h"
#include "Parsing/EnumValueParser.h"

namespace refureku
{
	class EnumParser
	{
		private:
			bool			_shouldCheckValidity	= false;
			bool			_isCurrentlyParsing		= false;
			CXCursor		_currentCursor			= clang_getNullCursor();

			EnumValueParser	_enumValueParser;

			std::optional<PropertyGroup>	isEnumValid(CXCursor currentCursor, ParsingInfo& parsingInfo)						noexcept;
			CXChildVisitResult				setAsCurrentEnumIfValid(CXCursor classAnnotationCursor, ParsingInfo& parsingInfo)	noexcept;
			void							endParsing(ParsingInfo& parsingInfo)												noexcept;

		public:
			EnumParser()					= default;
			EnumParser(EnumParser const&)	= default;
			EnumParser(EnumParser&&)		= default;
			~EnumParser()					= default;

			CXChildVisitResult	parse(CXCursor currentCursor, ParsingInfo& parsingInfo)					noexcept;
			void				startParsing(CXCursor currentCursor, ParsingInfo& parsingInfo)			noexcept;

			void				updateParsingState(CXCursor parent, ParsingInfo& parsingInfo)			noexcept;
			
			bool				isCurrentlyParsing()											const	noexcept;
	};
}