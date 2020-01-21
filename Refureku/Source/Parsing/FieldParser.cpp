#include "Parsing/FieldParser.h"

#include "Misc/Helpers.h"

using namespace refureku;

CXChildVisitResult FieldParser::parse(CXCursor cursor, ParsingInfo& parsingInfo) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (_shouldCheckValidity)
	{
		return addToCurrentClassIfValid(cursor, parsingInfo);
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult FieldParser::addToCurrentClassIfValid(CXCursor fieldAnnotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isFieldValid(fieldAnnotationCursor, parsingInfo))
	{
		if (parsingInfo.currentClass.has_value())
		{
			FieldInfo& field = parsingInfo.currentClass->fields.at(parsingInfo.accessSpecifier).emplace_back(FieldInfo(Helpers::getString(clang_getCursorDisplayName(_currentCursor)), std::move(*propertyGroup)));
			field.type = TypeInfo(clang_getCanonicalType(clang_getCursorType(_currentCursor)));

			return CXChildVisitResult::CXChildVisit_Recurse;
		}
		else
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
	}
	else
	{
		if (parsingInfo.propertyParser.getParsingError() == EParsingError::Count)
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo.propertyParser.getParsingError(), clang_getCursorLocation(fieldAnnotationCursor)));

			return parsingInfo.parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

std::optional<PropertyGroup> FieldParser::isFieldValid(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	_shouldCheckValidity = false;
	parsingInfo.propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo.propertyParser.getFieldProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
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