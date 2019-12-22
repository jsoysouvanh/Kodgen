#include "Parsing/MethodParser.h"

#include <assert.h>

#include "Helpers.h"

using namespace refureku;

CXChildVisitResult MethodParser::parse(CXCursor cursor, ParsingInfo& parsingInfo) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (_shouldCheckValidity)
	{
		return tryToAddMethod(cursor, parsingInfo);
	}

	CXCursorKind cursorKind = clang_getCursorKind(cursor);

	switch (cursorKind)
	{
		case CXCursorKind::CXCursor_CXXFinalAttr:
			parsingInfo._currentMethodInfo->qualifiers |= MethodInfo::EMethodQualifier::Final;
			break;

		case CXCursorKind::CXCursor_CXXOverrideAttr:
			parsingInfo._currentMethodInfo->qualifiers |= MethodInfo::EMethodQualifier::Override;
			break;

		case CXCursorKind::CXCursor_ParmDecl:
			//TODO: handle parameters here
			break;

		default:
			std::cout << "Unknown method sub cursor: " << Helpers::getString(clang_getCursorKindSpelling(clang_getCursorKind(cursor))) << std::endl;
			break;
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult MethodParser::tryToAddMethod(CXCursor const& methodAnnotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isMethodValid(methodAnnotationCursor, parsingInfo))
	{
		parsingInfo._currentMethodInfo = &parsingInfo._parsingResult.classes.back().methods.at(parsingInfo.accessSpecifier).emplace_back(MethodInfo(Helpers::getString(clang_getCursorDisplayName(_currentCursor)), std::move(*propertyGroup)));
		setupMethod(_currentCursor, *parsingInfo._currentMethodInfo);

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
			parsingInfo._parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo._propertyParser.getParsingError(), clang_getCursorLocation(methodAnnotationCursor)));

			return parsingInfo._parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

void MethodParser::setupMethod(CXCursor const& methodCursor, MethodInfo& methodInfo) noexcept
{
	CXType methodType = clang_getCursorType(methodCursor);

	assert(methodType.kind == CXTypeKind::CXType_FunctionProto);

	//Define return type
	methodInfo.returnType =	TypeInfo(clang_getCanonicalType(clang_getResultType(methodType)));

	//Define method qualifiers
	if (clang_CXXMethod_isDefaulted(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Default;
	}
	if (clang_CXXMethod_isStatic(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Static;
	}
	if (clang_CXXMethod_isVirtual(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Virtual;
	}
	if (clang_CXXMethod_isPureVirtual(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::PureVirtual;
	}
	if (clang_CXXMethod_isConst(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Const;
	}
	if (clang_Cursor_isFunctionInlined(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Inline;
	}
}

std::optional<PropertyGroup> MethodParser::isMethodValid(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	_shouldCheckValidity = false;
	parsingInfo._propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo._propertyParser.getMethodProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

void MethodParser::startParsing(CXCursor cursor) noexcept
{
	_isCurrentlyParsing		= true;
	_currentCursor			= cursor;
	_shouldCheckValidity	= true;

	//std::cout << "START METHOD" << std::endl;
}

void MethodParser::endParsing() noexcept
{
	_isCurrentlyParsing		= false;
	_currentCursor			= clang_getNullCursor();
	_shouldCheckValidity	= false;

	//std::cout << "END METHOD" << std::endl;
}

void MethodParser::updateParsingState(CXCursor parent) noexcept
{
	if (!clang_equalCursors(_currentCursor, parent))
	{
		endParsing();
	}
}

bool MethodParser::isCurrentlyParsing() const noexcept
{
	return _isCurrentlyParsing;
}