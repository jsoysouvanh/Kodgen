#include "Parsing/ClassParser.h"

#include <cassert>

#include "InfoStructures/NestedStructClassInfo.h"
#include "InfoStructures/NestedEnumInfo.h"
#include "Misc/Helpers.h"

using namespace kodgen;

void ClassParser::reset() noexcept
{
	EntityParser::reset();

	fieldParser.reset();
	methodParser.reset();

	_structOrClass = EntityInfo::EType::Count;
}

void ClassParser::setParsingInfo(ParsingInfo* info) noexcept
{
	EntityParser::setParsingInfo(info);

	fieldParser.setParsingInfo(info);
	methodParser.setParsingInfo(info);
}

DISABLE_WARNING_PUSH
DISABLE_WARNING_UNSCOPED_ENUM

CXChildVisitResult ClassParser::parse(CXCursor const& cursor) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (_shouldCheckValidity)
	{
		_shouldCheckValidity = false;
		return setAsCurrentEntityIfValid(cursor);
	}

	//Check for class field or method
	switch (cursor.kind)
	{
		case CXCursorKind::CXCursor_CXXFinalAttr:
			if (_parsingInfo->currentStructOrClass.has_value())
			{
				_parsingInfo->currentStructOrClass->qualifiers.isFinal = true;
			}
			break;

		case CXCursorKind::CXCursor_CXXAccessSpecifier:
			updateAccessSpecifier(cursor);
			break;

		case CXCursorKind::CXCursor_CXXBaseSpecifier:
			addToParents(cursor, *_parsingInfo);
			break;

		case CXCursorKind::CXCursor_Constructor:
			//TODO
			break;

		case CXCursorKind::CXCursor_VarDecl:	//For static fields
			[[fallthrough]];
		case CXCursorKind::CXCursor_FieldDecl:
			return parseField(cursor);

		case CXCursorKind::CXCursor_CXXMethod:
			return parseMethod(cursor);

		default:
			break;
	}

	return CXChildVisitResult::CXChildVisit_Continue;
}

DISABLE_WARNING_POP

CXChildVisitResult ClassParser::parseField(CXCursor fieldCursor) noexcept
{
	assert(fieldCursor.kind == CXCursorKind::CXCursor_FieldDecl || fieldCursor.kind == CXCursorKind::CXCursor_VarDecl);

	fieldParser.startParsing(fieldCursor);

	clang_visitChildren(fieldCursor, [](CXCursor c, CXCursor, CXClientData clientData)
						{
							return reinterpret_cast<FieldParser*>(clientData)->parse(c);

						}, &fieldParser);

	return fieldParser.endParsing();
}

CXChildVisitResult ClassParser::parseMethod(CXCursor methodCursor) noexcept
{
	assert(methodCursor.kind == CXCursorKind::CXCursor_CXXMethod);

	methodParser.startParsing(methodCursor);

	clang_visitChildren(methodCursor, [](CXCursor c, CXCursor, CXClientData clientData)
						{
							return reinterpret_cast<MethodParser*>(clientData)->parse(c);

						}, &methodParser);

	return methodParser.endParsing();
}

CXChildVisitResult ClassParser::setAsCurrentEntityIfValid(CXCursor const& classAnnotationCursor) noexcept
{
	if (opt::optional<PropertyGroup> propertyGroup = isEntityValid(classAnnotationCursor))
	{
		_parsingInfo->currentStructOrClass.emplace(StructClassInfo(getCurrentCursor(), std::move(*propertyGroup), std::move(_structOrClass)));
		initClassInfos(_parsingInfo->currentStructOrClass.value());

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (_parsingInfo->propertyParser.getParsingError() != EParsingError::Count)
		{
			_parsingInfo->parsingResult.errors.emplace_back(ParsingError(_parsingInfo->propertyParser.getParsingError(), clang_getCursorLocation(classAnnotationCursor)));
		}

		return CXChildVisitResult::CXChildVisit_Break;
	}
}

void ClassParser::initClassInfos(StructClassInfo& toInit) const noexcept
{
	CXType		classType	= clang_getCursorType(getCurrentCursor());
	std::string fullName	= Helpers::getString(clang_getTypeSpelling(classType));

	size_t namespacePos = fullName.find_last_of("::");

	//if (namespacePos != std::string::npos)
	//	toInit.nameSpace = std::string(fullName.cbegin(), fullName.cbegin() + namespacePos - 1);
}

opt::optional<PropertyGroup> ClassParser::isEntityValid(CXCursor const& currentCursor) noexcept
{
	_parsingInfo->propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		switch (_structOrClass)
		{
			case EntityInfo::EType::Class:
				return _parsingInfo->propertyParser.getClassProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
				break;

			case EntityInfo::EType::Struct:
				return _parsingInfo->propertyParser.getStructProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
				break;

			default:
				assert(false);	//Should never pass here
		}
	}

	return opt::nullopt;
}

void ClassParser::startClassParsing(CXCursor const& currentCursor)	noexcept
{
	EntityParser::startParsing(currentCursor);

	_parsingInfo->accessSpecifier	= EAccessSpecifier::Private;
	_structOrClass					= EntityInfo::EType::Class;
}

void ClassParser::startStructParsing(CXCursor const& currentCursor)	noexcept
{
	EntityParser::startParsing(currentCursor);

	_parsingInfo->accessSpecifier	= EAccessSpecifier::Public;
	_structOrClass					= EntityInfo::EType::Struct;
}

CXChildVisitResult ClassParser::endParsing()	noexcept
{
	_parsingInfo->accessSpecifier	= EAccessSpecifier::Invalid;
	_structOrClass					= EntityInfo::EType::Count;

	_parsingInfo->flushCurrentStructOrClass();

	return EntityParser::endParsing();
}

void ClassParser::updateAccessSpecifier(CXCursor const& cursor) const noexcept
{
	_parsingInfo->accessSpecifier = static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(cursor));
}

void ClassParser::addToParents(CXCursor cursor, ParsingInfo& parsingInfo) const noexcept
{
	assert(clang_getCursorKind(cursor) == CXCursorKind::CXCursor_CXXBaseSpecifier);

	if (parsingInfo.currentStructOrClass.has_value())
	{
		parsingInfo.currentStructOrClass->parents.emplace_back(StructClassInfo::ParentInfo{ static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(cursor)), TypeInfo(clang_getCursorType(cursor)) });
	}
}


//=========================================================================================

CXChildVisitResult ClassParser2::parse(CXCursor const& classCursor, ParsingContext const& parentContext, ClassParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the class parser
	assert(classCursor.kind == CXCursorKind::CXCursor_ClassDecl || classCursor.kind == CXCursorKind::CXCursor_StructDecl);

	//Init context
	pushContext(classCursor, parentContext, out_result);

	clang_visitChildren(classCursor, &ClassParser2::parseEntity, this);

	popContext();

	return (parentContext.parsingSettings->shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
}

FieldParsingResult ClassParser2::parseField(CXCursor const& fieldCursor, CXChildVisitResult& out_visitResult) noexcept
{
	FieldParsingResult fieldResult;

	out_visitResult = fieldParser.parse(fieldCursor, getContext(), fieldResult);

	return fieldResult;
}

MethodParsingResult	ClassParser2::parseMethod(CXCursor const& methodCursor, CXChildVisitResult& out_visitResult) noexcept
{
	MethodParsingResult methodResult;

	out_visitResult = methodParser.parse(methodCursor, getContext(), methodResult);

	return methodResult;
}

CXChildVisitResult ClassParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	ClassParser2*	parser	= reinterpret_cast<ClassParser2*>(clientData);
	ParsingContext&	context = parser->getContext();

	if (context.shouldCheckEntityValidity)
	{
		context.shouldCheckEntityValidity = false;

		//Set parsed struct/class in result if it is valid
		return parser->setParsedEntity(cursor);
	}
	else
	{
		CXChildVisitResult visitResult = CXChildVisitResult::CXChildVisit_Continue;

		switch (cursor.kind)
		{
			case CXCursorKind::CXCursor_CXXFinalAttr:
				if (parser->getParsingResult()->parsedClass.has_value())
				{
					parser->getParsingResult()->parsedClass->qualifiers.isFinal = true;
				}
				break;

			case CXCursorKind::CXCursor_CXXAccessSpecifier:
				parser->updateAccessSpecifier(cursor);
				break;

			case CXCursorKind::CXCursor_CXXBaseSpecifier:
				parser->addBaseClass(cursor);
				break;

			case CXCursorKind::CXCursor_Constructor:
				//TODO: Might handle this someday
				break;

			case CXCursorKind::CXCursor_StructDecl:
				[[fallthrough]];
			case CXCursorKind::CXCursor_ClassDecl:
				parser->addClassResult(parser->parseClass(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_EnumDecl:
				parser->addEnumResult(parser->parseEnum(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_VarDecl:	//For static fields
				[[fallthrough]];
			case CXCursorKind::CXCursor_FieldDecl:
				parser->addFieldResult(parser->parseField(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_CXXMethod:
				parser->addMethodResult(parser->parseMethod(cursor, visitResult));
				break;

			default:
				break;
		}

		return visitResult;
	}
}

ClassParsingResult ClassParser2::parseClass(CXCursor const& classCursor, CXChildVisitResult& out_visitResult) noexcept
{
	ClassParsingResult classResult;

	out_visitResult	= parse(classCursor, getContext(), classResult);

	return classResult;
}

EnumParsingResult ClassParser2::parseEnum(CXCursor const& enumCursor, CXChildVisitResult& out_visitResult) noexcept
{
	EnumParsingResult enumResult;

	//TODO
	//out_visitResult = parse(enumCursor, getContext(), enumResult);

	return enumResult;
}

void ClassParser2::pushContext(CXCursor const& classCursor, ParsingContext const& parentContext, ClassParsingResult& out_result) noexcept
{
	ParsingContext newContext;

	newContext.rootCursor					= classCursor;
	newContext.shouldCheckEntityValidity	= true;
	newContext.propertyParser				= parentContext.propertyParser;
	newContext.parsingSettings				= parentContext.parsingSettings;
	newContext.parsingResult				= &out_result;
	newContext.currentAccessSpecifier		= (classCursor.kind == CXCursorKind::CXCursor_ClassDecl) ? EAccessSpecifier::Private : EAccessSpecifier::Public;

	contextsStack.push(std::move(newContext));
}

CXChildVisitResult ClassParser2::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	ParsingContext& context = getContext();

	if (opt::optional<PropertyGroup> propertyGroup = getProperties(annotationCursor))
	{
		getParsingResult()->parsedClass.emplace(StructClassInfo(context.rootCursor, std::move(*propertyGroup), (context.rootCursor.kind == CXCursorKind::CXCursor_ClassDecl) ? EntityInfo::EType::Class : EntityInfo::EType::Struct));
		
		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (context.propertyParser->getParsingError() != EParsingError::Count)
		{
			context.parsingResult->errors.emplace_back(ParsingError(context.propertyParser->getParsingError(), clang_getCursorLocation(annotationCursor)));
		}

		return CXChildVisitResult::CXChildVisit_Break;
	}
}

opt::optional<PropertyGroup> ClassParser2::getProperties(CXCursor const& cursor) noexcept
{
	ParsingContext& context = getContext();

	context.propertyParser->clean();

	if (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		switch (context.rootCursor.kind)
		{
			case CXCursorKind::CXCursor_ClassDecl:
				return context.propertyParser->getClassProperties(Helpers::getString(clang_getCursorSpelling(cursor)));
				break;

			case CXCursorKind::CXCursor_StructDecl:
				return context.propertyParser->getStructProperties(Helpers::getString(clang_getCursorSpelling(cursor)));
				break;

			default:
				break;
		}
	}

	return opt::nullopt;
}

void ClassParser2::updateAccessSpecifier(CXCursor const& cursor) noexcept
{
	assert(cursor.kind == CXCursorKind::CXCursor_CXXAccessSpecifier);

	getContext().currentAccessSpecifier = static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(cursor));
}

void ClassParser2::addBaseClass(CXCursor cursor) noexcept
{
	assert(clang_getCursorKind(cursor) == CXCursorKind::CXCursor_CXXBaseSpecifier);

	if (getParsingResult()->parsedClass.has_value())
	{
		getParsingResult()->parsedClass->parents.emplace_back(StructClassInfo::ParentInfo{ static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(cursor)), TypeInfo(clang_getCursorType(cursor)) });
	}
}

void ClassParser2::addFieldResult(FieldParsingResult&& result) noexcept
{
	ParsingContext& context = getContext();

	if (result.parsedField.has_value() && getParsingResult()->parsedClass.has_value())
	{
		//Update field access specifier
		result.parsedField->accessSpecifier = context.currentAccessSpecifier;

		getParsingResult()->parsedClass->fields.emplace_back(std::move(result.parsedField).value());
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		context.parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}

void ClassParser2::addMethodResult(MethodParsingResult&& result) noexcept
{
	ParsingContext& context = getContext();

	if (result.parsedMethod.has_value() && getParsingResult()->parsedClass.has_value())
	{
		//Update method access specifier
		result.parsedMethod->accessSpecifier = context.currentAccessSpecifier;

		getParsingResult()->parsedClass->methods.emplace_back(std::move(result.parsedMethod).value());
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		context.parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}

void ClassParser2::addClassResult(ClassParsingResult&& result) noexcept
{
	ParsingContext& context = getContext();

	if (result.parsedClass.has_value() && getParsingResult()->parsedClass.has_value())
	{
		switch (result.parsedClass->entityType)
		{
			case EntityInfo::EType::Struct:
				getParsingResult()->parsedClass->nestedStructs.emplace_back(std::make_shared<NestedStructClassInfo>(std::move(result.parsedClass).value(), context.currentAccessSpecifier));
				break;

			case EntityInfo::EType::Class:
				getParsingResult()->parsedClass->nestedClasses.emplace_back(std::make_shared<NestedStructClassInfo>(std::move(result.parsedClass).value(), context.currentAccessSpecifier));
				break;

			default:
				assert(false);	//Should never reach this point
				break;
		}
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		context.parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}

void ClassParser2::addEnumResult(EnumParsingResult&& result) noexcept
{
	ParsingContext& context = getContext();

	if (result.parsedEnum.has_value() && getParsingResult()->parsedClass.has_value())
	{
		getParsingResult()->parsedClass->nestedEnums.emplace_back(NestedEnumInfo(std::move(result.parsedEnum).value(), context.currentAccessSpecifier));
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		context.parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}