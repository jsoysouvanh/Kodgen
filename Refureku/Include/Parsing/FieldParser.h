#pragma once

#include <clang-c/Index.h>
#include <optional>

#include "InfoStructures/ParsingInfo.h"
#include "Properties/PropertyGroup.h"

namespace refureku
{
	class FieldParser
	{
		private:
			bool		_shouldCheckValidity	= false;
			bool		_isCurrentlyParsing		= false;
			
			CXCursor	_currentCursor			= clang_getNullCursor();

			CXChildVisitResult				addToCurrentClassIfValid(CXCursor fieldAnnotationCursor, ParsingInfo& parsingInfo)	noexcept;
			std::optional<PropertyGroup>	isFieldValid(CXCursor currentCursor, ParsingInfo& parsingInfo)						noexcept;
			void							endParsing()																		noexcept;

		public:
			FieldParser()					= default;
			FieldParser(FieldParser const&) = default;
			FieldParser(FieldParser&&)		= default;
			~FieldParser()					= default;

			CXChildVisitResult	parse(CXCursor cursor, ParsingInfo& parsingInfo)		noexcept;

			void				startParsing(CXCursor cursor)							noexcept;
			void				updateParsingState(CXCursor parent)						noexcept;

			bool				isCurrentlyParsing()							const	noexcept;
	};
}