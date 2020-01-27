#include "Parsing/MethodParser.h"

#include <assert.h>

#include "Misc/Helpers.h"

using namespace refureku;

CXChildVisitResult MethodParser::parse(CXCursor cursor, ParsingInfo& parsingInfo) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (_shouldCheckValidity)
	{
		return addToCurrentClassIfValid(cursor, parsingInfo);
	}

	if (!parsingInfo.currentStructOrClass.has_value())
	{
		return CXChildVisitResult::CXChildVisit_Continue;
	}

	switch (clang_getCursorKind(cursor))
	{
		case CXCursorKind::CXCursor_CXXFinalAttr:
			parsingInfo.currentStructOrClass->methods.at(parsingInfo.accessSpecifier).back().qualifiers.Final = true;
			break;

		case CXCursorKind::CXCursor_CXXOverrideAttr:
			parsingInfo.currentStructOrClass->methods.at(parsingInfo.accessSpecifier).back().qualifiers.Override = true;
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

CXChildVisitResult MethodParser::addToCurrentClassIfValid(CXCursor const& methodAnnotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isMethodValid(methodAnnotationCursor, parsingInfo))
	{
		if (parsingInfo.currentStructOrClass.has_value())
		{
			MethodInfo& methodInfo = parsingInfo.currentStructOrClass->methods.at(parsingInfo.accessSpecifier).emplace_back(MethodInfo(Helpers::getString(clang_getCursorDisplayName(_currentCursor)), std::move(*propertyGroup)));
			setupMethod(_currentCursor, methodInfo);

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
			parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo.propertyParser.getParsingError(), clang_getCursorLocation(methodAnnotationCursor)));

			return parsingInfo.parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
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
		methodInfo.qualifiers.Default = true;
	}
	if (clang_CXXMethod_isStatic(methodCursor))
	{
		methodInfo.qualifiers.Static = true;
	}
	if (clang_CXXMethod_isVirtual(methodCursor))
	{
		methodInfo.qualifiers.Virtual = true;
	}
	if (clang_CXXMethod_isPureVirtual(methodCursor))
	{
		methodInfo.qualifiers.PureVirtual = true;
	}
	if (clang_CXXMethod_isConst(methodCursor))
	{
		methodInfo.qualifiers.Const = true;
	}
	if (clang_Cursor_isFunctionInlined(methodCursor))
	{
		methodInfo.qualifiers.Inline = true;
	}
}

std::optional<PropertyGroup> MethodParser::isMethodValid(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	_shouldCheckValidity = false;
	parsingInfo.propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo.propertyParser.getMethodProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

void MethodParser::startParsing(CXCursor cursor) noexcept
{
	_isCurrentlyParsing		= true;
	_currentCursor			= cursor;
	_shouldCheckValidity	= true;
}

void MethodParser::endParsing() noexcept
{
	_isCurrentlyParsing		= false;
	_currentCursor			= clang_getNullCursor();
	_shouldCheckValidity	= false;
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