#include "Parsing/EnumParser.h"

#include "Misc/Helpers.h"

using namespace refureku;

CXChildVisitResult EnumParser::parse(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	if (_enumValueParser.isCurrentlyParsing())
	{
		_enumValueParser.parse(currentCursor, parsingInfo);
	}
	else if (_shouldCheckValidity)	//Check for any annotation if the flag is raised
	{
		_shouldCheckValidity = false;

		return setAsCurrentEnumIfValid(currentCursor, parsingInfo);
	}

	switch (clang_getCursorKind(currentCursor))
	{
		case CXCursorKind::CXCursor_EnumConstantDecl:
			_enumValueParser.startParsing(currentCursor);
			break;

		default:
			return CXChildVisitResult::CXChildVisit_Continue;
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

void EnumParser::updateParsingState(CXCursor parent, ParsingInfo& parsingInfo) noexcept
{
	//Check if we finished parsing an enum value
	if (_enumValueParser.isCurrentlyParsing())
	{
		_enumValueParser.updateParsingState(parent, parsingInfo);
	}

	//Check if we finished parsing the enum
	if (clang_equalCursors(clang_getCursorSemanticParent(_currentCursor), parent))
	{
		endParsing(parsingInfo);
	}
}

void EnumParser::startParsing(CXCursor currentCursor) noexcept
{
	_isCurrentlyParsing		= true;
	_currentCursor			= currentCursor;
	_shouldCheckValidity	= true;
}

void EnumParser::endParsing(ParsingInfo& parsingInfo) noexcept
{
	_isCurrentlyParsing		= false;
	_currentCursor			= clang_getNullCursor();
	_shouldCheckValidity	= false;

	parsingInfo.flushCurrentEnum();
}

bool EnumParser::isCurrentlyParsing() const noexcept
{
	return _isCurrentlyParsing;
}

opt::optional<PropertyGroup> EnumParser::isEnumValid(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	parsingInfo.propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo.propertyParser.getEnumProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return opt::nullopt;
}

CXChildVisitResult EnumParser::setAsCurrentEnumIfValid(CXCursor classAnnotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (opt::optional<PropertyGroup> propertyGroup = isEnumValid(classAnnotationCursor, parsingInfo))
	{
		EnumInfo& enumInfo = parsingInfo.currentEnum.emplace(EnumInfo(Helpers::getString(clang_getCursorDisplayName(_currentCursor)), std::move(*propertyGroup)));

		CXType underlyingType				= clang_getEnumDeclIntegerType(_currentCursor);
		enumInfo.underlyingType				= Helpers::getString(clang_getTypeSpelling(underlyingType));
		enumInfo.canonicalUnderlyingType	= Helpers::getString(clang_getTypeSpelling(clang_getCanonicalType(underlyingType)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (parsingInfo.propertyParser.getParsingError() == EParsingError::Count)
		{
			endParsing(parsingInfo);
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo.propertyParser.getParsingError(), clang_getCursorLocation(classAnnotationCursor)));

			return parsingInfo.parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}