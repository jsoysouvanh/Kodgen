#include "Parsing/EnumParser.h"

#include "Misc/Helpers.h"

using namespace refureku;

CXChildVisitResult EnumParser::parse(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	std::string		cursorName	= Helpers::getString(clang_getCursorSpelling(currentCursor));

	//Check for any annotation if the flag is raised
	if (_shouldCheckValidity)
	{
		if (isEnumValid(currentCursor))
		{
			return CXChildVisitResult::CXChildVisit_Recurse;
		}
		else
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

void EnumParser::updateParsingState(CXCursor parent) noexcept
{

}

void EnumParser::startParsing(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	_isCurrentlyParsing		= true;
	_currentCursor			= currentCursor;
	_shouldCheckValidity	= true;

	//std::cout << "START ENUM" << std::endl;
}

void EnumParser::endParsing() noexcept
{
	_isCurrentlyParsing		= false;
	_currentCursor			= clang_getNullCursor();
	_shouldCheckValidity	= false;

	//std::cout << "END ENUM" << std::endl;
}

bool EnumParser::isCurrentlyParsing() const noexcept
{
	return _isCurrentlyParsing;
}

bool EnumParser::isEnumValid(CXCursor currentCursor) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	//std::string		cursorName	= ParsingInfo::getString(clang_getCursorSpelling(currentCursor));

	_shouldCheckValidity = false;

	return (cursorKind == CXCursorKind::CXCursor_AnnotateAttr && true/* TODO If notation is valid for an enum, add the enum*/);
}