#include "Parsing/FieldParser.h"

#include <Helpers.h>

using namespace refureku;

CXChildVisitResult FieldParser::parse(CXCursor cursor, ParsingInfo& parsingInfo) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (_shouldCheckValidity)
	{
		return tryToAddField(cursor, parsingInfo);
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult FieldParser::tryToAddField(CXCursor fieldAnnotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isFieldValid(fieldAnnotationCursor, parsingInfo))
	{
		FieldInfo& field = parsingInfo._parsingResult.classes.back().fields.at(parsingInfo.accessSpecifier).emplace_back(FieldInfo(Helpers::getString(clang_getCursorDisplayName(_currentCursor)), std::move(*propertyGroup)));
		field.type = TypeInfo(clang_getCanonicalType(clang_getCursorType(_currentCursor)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (parsingInfo._propertyParser.getParsingError() == EParsingError::Count)
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			parsingInfo._parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo._propertyParser.getParsingError(), clang_getCursorLocation(fieldAnnotationCursor)));

			return parsingInfo._parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

std::optional<PropertyGroup> FieldParser::isFieldValid(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	_shouldCheckValidity = false;
	parsingInfo._propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo._propertyParser.getFieldProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

void FieldParser::startParsing(CXCursor cursor) noexcept
{
	_isCurrentlyParsing		= true;
	_currentCursor			= cursor;
	_shouldCheckValidity	= true;

	//std::cout << "START FIELD: " << Helpers::getString(clang_getCursorDisplayName(cursor)) << std::endl;
}

void FieldParser::endParsing() noexcept
{
	_isCurrentlyParsing		= false;
	_currentCursor			= clang_getNullCursor();
	_shouldCheckValidity	= false;

	//std::cout << "END FIELD: " << Helpers::getString(clang_getCursorDisplayName(_currentCursor)) << std::endl;
}

void FieldParser::updateParsingState(CXCursor parent) noexcept
{
	if (!clang_equalCursors(_currentCursor, parent))
	{
		endParsing();
	}
}

bool FieldParser::isCurrentlyParsing() const noexcept
{
	return _isCurrentlyParsing;
}