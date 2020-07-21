#include "Parsing/ClassParser.h"

#include <cassert>

#include "Misc/Helpers.h"

using namespace kodgen;

void ClassParser::reset() noexcept
{
	EntityParser::reset();

	_fieldParser.reset();
	_methodParser.reset();

	_structOrClass = EntityInfo::EType::Count;
}

void ClassParser::setParsingInfo(ParsingInfo* info) noexcept
{
	EntityParser::setParsingInfo(info);

	_fieldParser.setParsingInfo(info);
	_methodParser.setParsingInfo(info);
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

	_fieldParser.startParsing(fieldCursor);

	clang_visitChildren(fieldCursor, [](CXCursor c, CXCursor, CXClientData clientData)
						{
							return reinterpret_cast<FieldParser*>(clientData)->parse(c);

						}, &_fieldParser);

	return _fieldParser.endParsing();
}

CXChildVisitResult ClassParser::parseMethod(CXCursor methodCursor) noexcept
{
	assert(methodCursor.kind == CXCursorKind::CXCursor_CXXMethod);

	_methodParser.startParsing(methodCursor);

	clang_visitChildren(methodCursor, [](CXCursor c, CXCursor, CXClientData clientData)
						{
							return reinterpret_cast<MethodParser*>(clientData)->parse(c);

						}, &_methodParser);

	return _methodParser.endParsing();
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
			_parsingInfo->parsingResult.parsingErrors.emplace_back(ParsingError(_parsingInfo->propertyParser.getParsingError(), clang_getCursorLocation(classAnnotationCursor)));
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

CXChildVisitResult ClassParser2::parseField(CXCursor const& fieldCursor) noexcept
{
	FieldParsingResult fieldResult;

	CXChildVisitResult childVisitResult = _fieldParser.parse(fieldCursor, *parsingContext.parsingSettings, *parsingContext.propertyParser, fieldResult);

	addFieldResult(fieldResult);

	return childVisitResult;
}

CXChildVisitResult ClassParser2::parse(CXCursor const& classCursor, ParsingSettings const& parsingSettings, PropertyParser& propertyParser, ClassParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the class parser
	assert(classCursor.kind == CXCursorKind::CXCursor_ClassDecl || classCursor.kind == CXCursorKind::CXCursor_StructDecl);

	//Init context
	initContext(classCursor, parsingSettings, propertyParser, out_result);

	clang_visitChildren(classCursor, &ClassParser2::parseEntity, this);

	return (parsingSettings.shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
}

CXChildVisitResult ClassParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	ClassParser2* parser = reinterpret_cast<ClassParser2*>(clientData);

	if (parser->parsingContext.shouldCheckEntityValidity)
	{
		parser->parsingContext.shouldCheckEntityValidity = false;

		//Set parsed struct/class in result if it is valid
		return parser->setParsedEntity(cursor);
	}
	else
	{
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
				//TODO
				break;

			case CXCursorKind::CXCursor_ClassDecl:
				//TODO handle nested class
				break;

			case CXCursorKind::CXCursor_StructDecl:
				//TODO handle nested struct
				break;

			case CXCursorKind::CXCursor_EnumDecl:
				//TODO handle nested enum
				break;

			case CXCursorKind::CXCursor_VarDecl:	//For static fields
				[[fallthrough]];
			case CXCursorKind::CXCursor_FieldDecl:
				return parser->parseField(cursor);

			//case CXCursorKind::CXCursor_CXXMethod:
			//	return parseMethod(cursor);

			default:
				break;
		}

		return CXChildVisitResult::CXChildVisit_Continue;
	}
}

void ClassParser2::initContext(CXCursor const& classCursor, ParsingSettings const& parsingSettings, PropertyParser& propertyParser, ClassParsingResult& out_result) noexcept
{
	parsingContext.rootCursor					= classCursor;
	parsingContext.shouldCheckEntityValidity	= true;
	parsingContext.propertyParser				= &propertyParser;
	parsingContext.parsingSettings				= &parsingSettings;
	parsingContext.parsingResult				= &out_result;
	parsingContext.currentAccessSpecifier		= (classCursor.kind == CXCursorKind::CXCursor_ClassDecl) ? EAccessSpecifier::Private : EAccessSpecifier::Public;
}

CXChildVisitResult ClassParser2::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	if (opt::optional<PropertyGroup> propertyGroup = getProperties(annotationCursor))
	{
		getParsingResult()->parsedClass.emplace(StructClassInfo(parsingContext.rootCursor, std::move(*propertyGroup), (parsingContext.rootCursor.kind == CXCursorKind::CXCursor_ClassDecl) ? EntityInfo::EType::Class : EntityInfo::EType::Struct));
		
		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (parsingContext.propertyParser->getParsingError() != EParsingError::Count)
		{
			parsingContext.parsingResult->errors.emplace_back(ParsingError(parsingContext.propertyParser->getParsingError(), clang_getCursorLocation(annotationCursor)));
		}

		return CXChildVisitResult::CXChildVisit_Break;
	}
}

opt::optional<PropertyGroup> ClassParser2::getProperties(CXCursor const& cursor) noexcept
{
	parsingContext.propertyParser->clean();

	if (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		switch (parsingContext.rootCursor.kind)
		{
			case CXCursorKind::CXCursor_ClassDecl:
				return parsingContext.propertyParser->getClassProperties(Helpers::getString(clang_getCursorSpelling(cursor)));
				break;

			case CXCursorKind::CXCursor_StructDecl:
				return parsingContext.propertyParser->getStructProperties(Helpers::getString(clang_getCursorSpelling(cursor)));
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

	parsingContext.currentAccessSpecifier = static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(cursor));
}

void ClassParser2::addBaseClass(CXCursor cursor) noexcept
{
	assert(clang_getCursorKind(cursor) == CXCursorKind::CXCursor_CXXBaseSpecifier);

	if (getParsingResult()->parsedClass.has_value())
	{
		getParsingResult()->parsedClass->parents.emplace_back(StructClassInfo::ParentInfo{ static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(cursor)), TypeInfo(clang_getCursorType(cursor)) });
	}
}

void ClassParser2::addFieldResult(FieldParsingResult& result) noexcept
{
	if (result.parsedField.has_value())
	{
		//Update field access specifier
		result.parsedField->accessSpecifier = parsingContext.currentAccessSpecifier;

		if (getParsingResult()->parsedClass.has_value())
		{
			getParsingResult()->parsedClass->fields.emplace_back(result.parsedField.value());
		}
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		getParsingResult()->errors.insert(getParsingResult()->errors.cend(), result.errors.cbegin(), result.errors.cend());
	}
}