#include "Parsing/NamespaceParser.h"

#include "Misc/Helpers.h"

using namespace kodgen;

opt::optional<PropertyGroup> NamespaceParser::isEntityValid(CXCursor const& currentCursor) noexcept
{
	return opt::nullopt;
}

CXChildVisitResult NamespaceParser::setAsCurrentEntityIfValid(CXCursor const& classAnnotationCursor) noexcept
{
	return CXChildVisitResult::CXChildVisit_Recurse;
}

void NamespaceParser::addToParents(CXCursor cursor, ParsingInfo& parsingInfo) const noexcept
{

}

void NamespaceParser::updateAccessSpecifier(CXCursor const& cursor) const noexcept
{

}

CXChildVisitResult NamespaceParser::endParsing() noexcept
{
	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult NamespaceParser::parse(CXCursor const& cursor) noexcept
{
	std::cout << Helpers::getString(clang_getCursorKindSpelling(clang_getCursorKind(cursor))) << " --> " << Helpers::getString(clang_getCursorDisplayName(cursor)) << std::endl;

	if (_shouldCheckValidity)	//Check for any annotation attribute if the flag is raised
	{
		_shouldCheckValidity = false;
		return setAsCurrentEntityIfValid(cursor);
	}

	//Check for class field or method
	//switch (cursor.kind)
	//{
	//	case CXCursorKind::CXCursor_CXXFinalAttr:
	//		if (_parsingInfo->currentStructOrClass.has_value())
	//		{
	//			_parsingInfo->currentStructOrClass->qualifiers.isFinal = true;
	//		}
	//		break;

	//	case CXCursorKind::CXCursor_CXXAccessSpecifier:
	//		updateAccessSpecifier(cursor);
	//		break;

	//	case CXCursorKind::CXCursor_CXXBaseSpecifier:
	//		addToParents(cursor, *_parsingInfo);
	//		break;

	//	case CXCursorKind::CXCursor_Constructor:
	//		//TODO
	//		break;

	//	case CXCursorKind::CXCursor_VarDecl:	//For static fields
	//		[[fallthrough]];
	//	case CXCursorKind::CXCursor_FieldDecl:
	//		return parseField(cursor);

	//	case CXCursorKind::CXCursor_CXXMethod:
	//		return parseMethod(cursor);

	//	default:
	//		break;
	//}

	return CXChildVisitResult::CXChildVisit_Continue;
}

void NamespaceParser::reset() noexcept
{
	
}

void NamespaceParser::setParsingInfo(ParsingInfo* info) noexcept
{
}

void NamespaceParser::startClassParsing(CXCursor const& currentCursor) noexcept
{

}

void NamespaceParser::startStructParsing(CXCursor const& currentCursor) noexcept
{

}