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

CXChildVisitResult MethodParser2::parse(CXCursor const& methodCursor, ParsingContext const&	parentContext, MethodParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the method parser
	assert(methodCursor.kind == CXCursorKind::CXCursor_CXXMethod);	// /!\ might have to add CXCursor_FunctionDecl and CXCursor_FunctionTemplate

	//Init context
	pushContext(methodCursor, parentContext, out_result);

	clang_visitChildren(methodCursor, &MethodParser2::parseEntity, this);

	popContext();

	return (parentContext.parsingSettings->shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
}

void MethodParser2::pushContext(CXCursor const& methodCursor, ParsingContext const&	parentContext, MethodParsingResult& out_result) noexcept
{
	ParsingContext newContext;

	newContext.rootCursor					= methodCursor;
	newContext.shouldCheckEntityValidity	= true;
	newContext.propertyParser				= parentContext.propertyParser;
	newContext.parsingSettings				= parentContext.parsingSettings;
	newContext.parsingResult				= &out_result;

	contextsStack.push(std::move(newContext));
}

CXChildVisitResult MethodParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	MethodParser2*	parser	= reinterpret_cast<MethodParser2*>(clientData);
	ParsingContext&	context = parser->getContext();

	if (context.shouldCheckEntityValidity)
	{
		context.shouldCheckEntityValidity = false;

		//Set parsed method in result if it is valid
		return parser->setParsedEntity(cursor);
	}
	else
	{
		switch (clang_getCursorKind(cursor))
		{
			case CXCursorKind::CXCursor_CXXFinalAttr:
				if (parser->getParsingResult()->parsedMethod.has_value())
				{
					parser->getParsingResult()->parsedMethod->qualifiers.isFinal = true;
				}
				break;

			case CXCursorKind::CXCursor_CXXOverrideAttr:
				if (parser->getParsingResult()->parsedMethod.has_value())
				{
					parser->getParsingResult()->parsedMethod->qualifiers.isOverride = true;
				}
				break;

			case CXCursorKind::CXCursor_ParmDecl:
				if (parser->getParsingResult()->parsedMethod.has_value())
				{
					parser->getParsingResult()->parsedMethod->parameters.emplace_back(MethodParamInfo{TypeInfo(clang_getCursorType(cursor)), Helpers::getString(clang_getCursorDisplayName(cursor))});
				}
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
	MethodParsingResult*	result	= getParsingResult();
	ParsingContext&			context	= getContext();

	if (opt::optional<PropertyGroup> propertyGroup = getProperties(annotationCursor))
	{
		//Set the parsed entity in the result & initialize its information from the method cursor
		initializeMethodInfo(result->parsedMethod.emplace(MethodInfo(context.rootCursor, std::move(*propertyGroup))));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else if (context.propertyParser->getParsingError() != EParsingError::Count)
	{
		//Fatal parsing error occured
		result->errors.emplace_back(ParsingError(context.propertyParser->getParsingError(), clang_getCursorLocation(context.rootCursor)));
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

opt::optional<PropertyGroup> MethodParser2::getProperties(CXCursor const& cursor) noexcept
{
	ParsingContext& context	= getContext();

	context.propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
				context.propertyParser->getMethodProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
				opt::nullopt;
}

void MethodParser2::initializeMethodInfo(MethodInfo& methodInfo) noexcept
{
	ParsingContext& context		= getContext();
	CXType			methodType	= clang_getCursorType(context.rootCursor);

	assert(methodType.kind == CXTypeKind::CXType_FunctionProto);

	//Define prototype
	methodInfo.prototype = Helpers::getString(clang_getTypeSpelling(methodType));

	//Define return type
	methodInfo.returnType =	TypeInfo(clang_getResultType(methodType));

	//Define method qualifiers
	if (clang_CXXMethod_isDefaulted(context.rootCursor))
	{
		methodInfo.qualifiers.isDefault = true;
	}
	if (clang_CXXMethod_isStatic(context.rootCursor))
	{
		methodInfo.qualifiers.isStatic = true;
	}
	if (clang_CXXMethod_isVirtual(context.rootCursor))
	{
		methodInfo.qualifiers.isVirtual = true;
	}
	if (clang_CXXMethod_isPureVirtual(context.rootCursor))
	{
		methodInfo.qualifiers.isPureVirtual = true;
	}
	if (clang_CXXMethod_isConst(context.rootCursor))
	{
		methodInfo.qualifiers.isConst = true;
	}
	if (clang_Cursor_isFunctionInlined(context.rootCursor))
	{
		methodInfo.qualifiers.isInline = true;
	}
}