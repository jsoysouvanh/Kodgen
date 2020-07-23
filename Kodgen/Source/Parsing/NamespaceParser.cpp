#include "Parsing/NamespaceParser.h"

#include "Misc/Helpers.h"

using namespace kodgen;

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

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (getContext().propertyParser->getParsingError() != EParsingError::Count)
		{
			getContext().parsingResult->errors.emplace_back(ParsingError(getContext().propertyParser->getParsingError(), clang_getCursorLocation(annotationCursor)));
		}

		return CXChildVisitResult::CXChildVisit_Break;
	}
}

opt::optional<PropertyGroup> NamespaceParser2::getProperties(CXCursor const& cursor) noexcept
{
	ParsingContext& context = getContext();

	context.propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
				context.propertyParser->getNamespaceProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
				opt::nullopt;
}

CXChildVisitResult NamespaceParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	NamespaceParser2*	parser	= reinterpret_cast<NamespaceParser2*>(clientData);
	ParsingContext&		context = parser->getContext();

	//std::cout << "NAMESPACE => " << Helpers::getString(clang_getCursorKindSpelling(cursor.kind)) << " " << Helpers::getString(clang_getCursorDisplayName(cursor)) << std::endl;

	if (context.shouldCheckEntityValidity)
	{
		context.shouldCheckEntityValidity = false;

		return parser->setParsedEntity(cursor);
	}
	else
	{
		CXChildVisitResult visitResult = CXChildVisitResult::CXChildVisit_Continue;

		switch (cursor.kind)
		{
			case CXCursorKind::CXCursor_Namespace:
				parser->addNamespaceResult(parser->parseNamespace(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_StructDecl:
				[[fallthrough]];
			case CXCursorKind::CXCursor_ClassDecl:
				parser->addClassResult(parser->parseClass(cursor, visitResult));

			case CXCursorKind::CXCursor_EnumDecl:
				parser->addEnumResult(parser->parseEnum(cursor, visitResult));
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

		return visitResult;
	}
}

NamespaceParsingResult NamespaceParser2::parseNamespace(CXCursor const& namespaceCursor, CXChildVisitResult& out_visitResult) noexcept
{
	NamespaceParsingResult namespaceResult;
	
	out_visitResult	= NamespaceParser2::parse(namespaceCursor, getContext(), namespaceResult);

	return namespaceResult;
}

void NamespaceParser2::addNamespaceResult(NamespaceParsingResult&& result) noexcept
{
	ParsingContext& context = getContext();

	if (result.parsedNamespace.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		getParsingResult()->parsedNamespace->namespaces.emplace_back(std::move(result.parsedNamespace).value());
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		getContext().parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}

void NamespaceParser2::addClassResult(ClassParsingResult&& result) noexcept
{
	if (result.parsedClass.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		switch (result.parsedClass->entityType)
		{
			case EntityInfo::EType::Struct:
				getParsingResult()->parsedNamespace->structs.emplace_back(std::move(result.parsedClass).value());
				break;

			case EntityInfo::EType::Class:
				getParsingResult()->parsedNamespace->classes.emplace_back(std::move(result.parsedClass).value());
				break;

			default:
				assert(false);	//Should never reach this line
				break;
		}
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		getContext().parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}

void NamespaceParser2::addEnumResult(EnumParsingResult&& result) noexcept
{
	if (result.parsedEnum.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		getParsingResult()->parsedNamespace->enums.emplace_back(std::move(result.parsedEnum).value());
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		getContext().parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}