#pragma once

#include <clang-c/Index.h>

#include "InfoStructures/ParsingInfo.h"

namespace refureku
{
	class EnumValueParser
	{
		private:
			bool		_shouldCheckValidity	= false;
			bool		_isCurrentlyParsing		= false;
			CXCursor	_currentCursor			= clang_getNullCursor();

			std::optional<PropertyGroup>	isValidEnumValueAnnotation(CXCursor currentCursor, ParsingInfo& parsingInfo)					noexcept;
			CXChildVisitResult				addToCurrentEnumIfValid(CXCursor annotationCursor, ParsingInfo& parsingInfo)					noexcept;
			void							endParsing(ParsingInfo& parsingInfo)															noexcept;
			void							addToCurrentEnumValue(CXCursor enumValueCursor, PropertyGroup&& pg, ParsingInfo& parsingInfo)	noexcept;

		public:
			EnumValueParser()						= default;
			EnumValueParser(EnumValueParser const&)	= default;
			EnumValueParser(EnumValueParser&&)		= default;
			~EnumValueParser()						= default;

			CXChildVisitResult	parse(CXCursor currentCursor, ParsingInfo& parsingInfo)					noexcept;
			void				startParsing(CXCursor currentCursor, ParsingInfo& parsingInfo)			noexcept;

			void				updateParsingState(CXCursor parent, ParsingInfo& parsingInfo)			noexcept;

			bool				isCurrentlyParsing()											const	noexcept;
	};
}