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

//=========================================================================================

#include "Parsing/NamespaceParser.h"

#include <cassert>

CXChildVisitResult NamespaceParser2::parse(CXCursor const& namespaceCursor, ParsingContext const& parentContext, NamespaceParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the namespace parser
	assert(namespaceCursor.kind == CXCursorKind::CXCursor_Namespace);

	//Init context
	pushContext(namespaceCursor, parentContext, out_result);

	clang_visitChildren(namespaceCursor, &NamespaceParser2::parseEntity, this);

	popContext();

	return (parentContext.parsingSettings->shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
}

void NamespaceParser2::pushContext(CXCursor const& namespaceCursor, ParsingContext const& parentContext, NamespaceParsingResult& out_result) noexcept
{
	//Add a new context to the contexts stack
	ParsingContext newContext;

	newContext.rootCursor					= namespaceCursor;
	newContext.shouldCheckEntityValidity	= true;
	newContext.propertyParser				= parentContext.propertyParser;
	newContext.parsingSettings				= parentContext.parsingSettings;
	newContext.parsingResult				= &out_result;

	contextsStack.push(std::move(newContext));
}

CXChildVisitResult NamespaceParser2::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	if (opt::optional<PropertyGroup> propertyGroup = getProperties(annotationCursor))
	{
		getParsingResult()->parsedNamespace.emplace(NamespaceInfo(getContext().rootCursor, std::move(*propertyGroup)));
	}
	else if (getContext().propertyParser->getParsingError() != EParsingError::Count)
	{
		getContext().parsingResult->errors.emplace_back(ParsingError(getContext().propertyParser->getParsingError(), clang_getCursorLocation(annotationCursor)));

		return CXChildVisitResult::CXChildVisit_Break;
	}

	//A namespace without properties is still inspected
	return CXChildVisitResult::CXChildVisit_Recurse;
}

opt::optional<PropertyGroup> NamespaceParser2::getProperties(CXCursor const& cursor) noexcept
{
	getContext().propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
				getContext().propertyParser->getNamespaceProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
				PropertyGroup();
}

CXChildVisitResult NamespaceParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	NamespaceParser2* parser = reinterpret_cast<NamespaceParser2*>(clientData);

	if (parser->getContext().shouldCheckEntityValidity)
	{
		parser->getContext().shouldCheckEntityValidity = false;

		//Set the parsed namespace in result if it is valid
		return parser->setParsedEntity(cursor);
	}
	else
	{
		//At this point, the result must contain a valid namespace
		assert(parser->getParsingResult()->parsedNamespace.has_value());

		//TODO: Log what's here

		switch (cursor.kind)
		{
			case CXCursorKind::CXCursor_ClassDecl:
				//TODO: Handle nested class
				break;

			case CXCursorKind::CXCursor_StructDecl:
				//TODO: Handle nested struct
				break;

			case CXCursorKind::CXCursor_EnumDecl:
				//TODO: Handle nested enum
				break;

			case CXCursorKind::CXCursor_VarDecl:	//For static fields
				[[fallthrough]];
			case CXCursorKind::CXCursor_FieldDecl:
				//TODO: return parser->parseField(cursor);
				break;

			case CXCursorKind::CXCursor_CXXMethod:
				//TODO: return parser->parseMethod(cursor);
				break;

			default:
				break;
		}

		return CXChildVisitResult::CXChildVisit_Continue;
	}
}