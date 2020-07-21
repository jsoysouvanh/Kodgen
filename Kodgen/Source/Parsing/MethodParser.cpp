#include "Parsing/MethodParser.h"

#include <assert.h>

#include "Misc/Helpers.h"
#include "InfoStructures/MethodParamInfo.h"

using namespace kodgen;

CXChildVisitResult MethodParser::parse(CXCursor const& cursor) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (_shouldCheckValidity)
	{
		return setAsCurrentEntityIfValid(cursor);
	}

	if (!_parsingInfo->currentStructOrClass.has_value())
	{
		return CXChildVisitResult::CXChildVisit_Break;
	}

	switch (clang_getCursorKind(cursor))
	{
		case CXCursorKind::CXCursor_CXXFinalAttr:
			_parsingInfo->currentStructOrClass->methods.back().qualifiers.isFinal = true;
			break;

		case CXCursorKind::CXCursor_CXXOverrideAttr:
			_parsingInfo->currentStructOrClass->methods.back().qualifiers.isOverride = true;
			break;

		case CXCursorKind::CXCursor_ParmDecl:
			_parsingInfo->currentStructOrClass->methods.back().parameters.emplace_back(MethodParamInfo{ TypeInfo(clang_getCursorType(cursor)), Helpers::getString(clang_getCursorDisplayName(cursor)) });
			break;

		default:
			//std::cout << "Unknown method sub cursor: " << Helpers::getString(clang_getCursorKindSpelling(clang_getCursorKind(cursor))) << std::endl;
			break;
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult MethodParser::setAsCurrentEntityIfValid(CXCursor const& methodAnnotationCursor) noexcept
{
	if (opt::optional<PropertyGroup> propertyGroup = isEntityValid(methodAnnotationCursor))
	{
		if (_parsingInfo->currentStructOrClass.has_value())
		{

			MethodInfo& methodInfo = _parsingInfo->currentStructOrClass->methods.emplace_back(MethodInfo(getCurrentCursor(), std::move(*propertyGroup)));
			setupMethod(getCurrentCursor(), methodInfo);

			return CXChildVisitResult::CXChildVisit_Recurse;
		}
	}
	else if (_parsingInfo->propertyParser.getParsingError() != EParsingError::Count)
	{
		//Fatal parsing error occured
		_parsingInfo->parsingResult.errors.emplace_back(ParsingError(_parsingInfo->propertyParser.getParsingError(), clang_getCursorLocation(methodAnnotationCursor)));
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

void MethodParser::setupMethod(CXCursor const& methodCursor, MethodInfo& methodInfo) noexcept
{
	CXType methodType = clang_getCursorType(methodCursor);

	assert(methodType.kind == CXTypeKind::CXType_FunctionProto);

	//Method access specifier
	methodInfo.accessSpecifier = _parsingInfo->accessSpecifier;

	//Define prototype
	methodInfo.prototype = Helpers::getString(clang_getTypeSpelling(methodType));

	//Define return type
	methodInfo.returnType =	TypeInfo(clang_getResultType(methodType));

	//Define method qualifiers
	if (clang_CXXMethod_isDefaulted(methodCursor))
	{
		methodInfo.qualifiers.isDefault = true;
	}
	if (clang_CXXMethod_isStatic(methodCursor))
	{
		methodInfo.qualifiers.isStatic = true;
	}
	if (clang_CXXMethod_isVirtual(methodCursor))
	{
		methodInfo.qualifiers.isVirtual = true;
	}
	if (clang_CXXMethod_isPureVirtual(methodCursor))
	{
		methodInfo.qualifiers.isPureVirtual = true;
	}
	if (clang_CXXMethod_isConst(methodCursor))
	{
		methodInfo.qualifiers.isConst = true;
	}
	if (clang_Cursor_isFunctionInlined(methodCursor))
	{
		methodInfo.qualifiers.isInline = true;
	}
}

opt::optional<PropertyGroup> MethodParser::isEntityValid(CXCursor const& currentCursor) noexcept
{
	_shouldCheckValidity = false;
	_parsingInfo->propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return _parsingInfo->propertyParser.getMethodProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return opt::nullopt;
}

//=========================================================================================

CXChildVisitResult MethodParser2::parse(CXCursor const& methodCursor, ParsingSettings const& parsingSettings, PropertyParser& propertyParser, MethodParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the method parser
	assert(methodCursor.kind == CXCursorKind::CXCursor_CXXMethod);	// /!\ might have to add CXCursor_FunctionDecl and CXCursor_FunctionTemplate

	//Init context
	initContext(methodCursor, parsingSettings, propertyParser, out_result);

	clang_visitChildren(methodCursor, &MethodParser2::parseEntity, this);

	return (parsingSettings.shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
}

void MethodParser2::initContext(CXCursor const& methodCursor, ParsingSettings const& parsingSettings, PropertyParser& propertyParser, MethodParsingResult& out_result) noexcept
{
	parsingContext.rootCursor					= methodCursor;
	parsingContext.shouldCheckEntityValidity	= true;
	parsingContext.propertyParser				= &propertyParser;
	parsingContext.parsingSettings				= &parsingSettings;
	parsingContext.parsingResult				= &out_result;
}

CXChildVisitResult MethodParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	MethodParser2* parser = reinterpret_cast<MethodParser2*>(clientData);

	if (parser->parsingContext.shouldCheckEntityValidity)
	{
		parser->parsingContext.shouldCheckEntityValidity = false;

		//Set parsed method in result if it is valid
		return parser->setParsedEntity(cursor);
	}
	else
	{
		//At this point, the result must contain a valid method
		assert(parser->getParsingResult()->parsedMethod.has_value());

		switch (clang_getCursorKind(cursor))
		{
			case CXCursorKind::CXCursor_CXXFinalAttr:
				parser->getParsingResult()->parsedMethod->qualifiers.isFinal = true;
				break;

			case CXCursorKind::CXCursor_CXXOverrideAttr:
				parser->getParsingResult()->parsedMethod->qualifiers.isOverride = true;
				break;

			case CXCursorKind::CXCursor_ParmDecl:
				parser->getParsingResult()->parsedMethod->parameters.emplace_back(MethodParamInfo{ TypeInfo(clang_getCursorType(cursor)), Helpers::getString(clang_getCursorDisplayName(cursor)) });
				break;

			default:
				//std::cout << "Unknown method sub cursor: " << Helpers::getString(clang_getCursorKindSpelling(clang_getCursorKind(cursor))) << std::endl;
				break;
		}

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
}

CXChildVisitResult MethodParser2::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	MethodParsingResult* result = getParsingResult();

	if (opt::optional<PropertyGroup> propertyGroup = getProperties(annotationCursor))
	{
		//Set the parsed entity in the result & initialize its information from the method cursor
		initializeMethodInfo(result->parsedMethod.emplace(MethodInfo(parsingContext.rootCursor, std::move(*propertyGroup))));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else if (parsingContext.propertyParser->getParsingError() != EParsingError::Count)
	{
		//Fatal parsing error occured
		result->errors.emplace_back(ParsingError(parsingContext.propertyParser->getParsingError(), clang_getCursorLocation(parsingContext.rootCursor)));
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

opt::optional<PropertyGroup> MethodParser2::getProperties(CXCursor const& cursor) noexcept
{
	parsingContext.propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
				parsingContext.propertyParser->getMethodProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
				opt::nullopt;
}

void MethodParser2::initializeMethodInfo(MethodInfo& methodInfo) noexcept
{
	CXType methodType = clang_getCursorType(parsingContext.rootCursor);

	assert(methodType.kind == CXTypeKind::CXType_FunctionProto);

	//Define prototype
	methodInfo.prototype = Helpers::getString(clang_getTypeSpelling(methodType));

	//Define return type
	methodInfo.returnType =	TypeInfo(clang_getResultType(methodType));

	//Define method qualifiers
	if (clang_CXXMethod_isDefaulted(parsingContext.rootCursor))
	{
		methodInfo.qualifiers.isDefault = true;
	}
	if (clang_CXXMethod_isStatic(parsingContext.rootCursor))
	{
		methodInfo.qualifiers.isStatic = true;
	}
	if (clang_CXXMethod_isVirtual(parsingContext.rootCursor))
	{
		methodInfo.qualifiers.isVirtual = true;
	}
	if (clang_CXXMethod_isPureVirtual(parsingContext.rootCursor))
	{
		methodInfo.qualifiers.isPureVirtual = true;
	}
	if (clang_CXXMethod_isConst(parsingContext.rootCursor))
	{
		methodInfo.qualifiers.isConst = true;
	}
	if (clang_Cursor_isFunctionInlined(parsingContext.rootCursor))
	{
		methodInfo.qualifiers.isInline = true;
	}
}