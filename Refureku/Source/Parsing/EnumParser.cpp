#include "Parsing/EnumParser.h"

#include "Helpers.h"

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

bool EnumParser::isEnumValid(CXCursor currentCursor) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	//std::string		cursorName	= ParsingInfo::getString(clang_getCursorSpelling(currentCursor));

	_shouldCheckValidity = false;

	return (cursorKind == CXCursorKind::CXCursor_AnnotateAttr && true/* TODO If notation is valid for an enum, add the enum*/);
}