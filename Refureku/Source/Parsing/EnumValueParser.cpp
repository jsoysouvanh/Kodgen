#include "Parsing/EnumValueParser.h"

#include "Misc/Helpers.h"

using namespace refureku;

CXChildVisitResult EnumValueParser::parse(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	if (_shouldCheckValidity)
	{
		_shouldCheckValidity = false;

		switch (clang_getCursorKind(currentCursor))
		{
			case CXCursorKind::CXCursor_AnnotateAttr:
				return addToCurrentEnumIfValid(currentCursor, parsingInfo);

			case CXCursorKind::CXCursor_IntegerLiteral:
				[[fallthrough]];
			case CXCursorKind::CXCursor_BinaryOperator:
				[[fallthrough]];
			case CXCursorKind::CXCursor_UnexposedExpr:
				addToCurrentEnumValue(_currentCursor, PropertyGroup(), parsingInfo);
				break;

			default:
				return CXChildVisitResult::CXChildVisit_Continue;
		}

		return CXChildVisitResult::CXChildVisit_Recurse;
	}

	return CXChildVisitResult::CXChildVisit_Continue;
}

void EnumValueParser::startParsing(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	_isCurrentlyParsing		= true;
	_currentCursor			= currentCursor;
	_shouldCheckValidity	= true;
}

void EnumValueParser::endParsing(ParsingInfo& parsingInfo) noexcept
{
	_isCurrentlyParsing		= false;
	_currentCursor			= clang_getNullCursor();
	_shouldCheckValidity	= false;
}

void EnumValueParser::addToCurrentEnumValue(CXCursor enumValueCursor, PropertyGroup&& pg, ParsingInfo& parsingInfo) noexcept
{
	EnumValueInfo& enumValueInfo = parsingInfo.currentEnum->enumValues.emplace_back(EnumValueInfo(Helpers::getString(clang_getCursorSpelling(enumValueCursor)), std::forward<PropertyGroup>(pg)));

	enumValueInfo.defaultValue = clang_getEnumConstantDeclValue(enumValueCursor);
}

void EnumValueParser::updateParsingState(CXCursor parent, ParsingInfo& parsingInfo) noexcept
{
	if (!clang_equalCursors(_currentCursor, parent))
	{
		//If the checkValidity flag is set, the current cursor is necessary true
		if (_shouldCheckValidity)
		{
			addToCurrentEnumValue(_currentCursor, PropertyGroup(), parsingInfo);
		}

		endParsing(parsingInfo);
	}
}

std::optional<PropertyGroup> EnumValueParser::isValidEnumValueAnnotation(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	parsingInfo.propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo.propertyParser.getEnumValueProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

CXChildVisitResult EnumValueParser::addToCurrentEnumIfValid(CXCursor annotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isValidEnumValueAnnotation(annotationCursor, parsingInfo))
	{
		if (parsingInfo.currentEnum.has_value())
		{
			addToCurrentEnumValue(_currentCursor, std::move(*propertyGroup), parsingInfo);

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
			parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo.propertyParser.getParsingError(), clang_getCursorLocation(annotationCursor)));

			return parsingInfo.parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

bool EnumValueParser::isCurrentlyParsing() const noexcept
{
	return _isCurrentlyParsing;
}