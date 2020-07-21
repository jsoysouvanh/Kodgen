#include "Parsing/FieldParser.h"

#include <cassert>

#include "Misc/Helpers.h"

using namespace kodgen;

CXChildVisitResult FieldParser::parse(CXCursor const& cursor) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (_shouldCheckValidity)
	{
		return setAsCurrentEntityIfValid(cursor);
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

CXChildVisitResult FieldParser::setAsCurrentEntityIfValid(CXCursor const& fieldAnnotationCursor) noexcept
{
	if (opt::optional<PropertyGroup> propertyGroup = isEntityValid(fieldAnnotationCursor))
	{
		if (_parsingInfo->currentStructOrClass.has_value())
		{
			FieldInfo& field = _parsingInfo->currentStructOrClass->fields.emplace_back(FieldInfo(getCurrentCursor(), std::move(*propertyGroup)));
			
			field.accessSpecifier = _parsingInfo->accessSpecifier;
			field.type = TypeInfo(clang_getCursorType(getCurrentCursor()));
			field.qualifiers.isStatic = (clang_getCursorKind(getCurrentCursor()) == CXCursorKind::CXCursor_VarDecl);

			if (!field.qualifiers.isStatic)
			{
				field.qualifiers.isMutable = clang_CXXField_isMutable(getCurrentCursor());

				field.memoryOffset = clang_Cursor_getOffsetOfField(getCurrentCursor());

				// assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_Invalid);	<- Assert here on travis for some reasons...
				assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_Incomplete);
				assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_Dependent);
				assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_InvalidFieldName);

				field.memoryOffset /= 8;	//From bits to bytes
			}
		}
	}
	else if (_parsingInfo->propertyParser.getParsingError() != EParsingError::Count)
	{
		//Fatal parsing error occured
		_parsingInfo->parsingResult.errors.emplace_back(ParsingError(_parsingInfo->propertyParser.getParsingError(), clang_getCursorLocation(fieldAnnotationCursor)));
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

opt::optional<PropertyGroup> FieldParser::isEntityValid(CXCursor const& currentCursor) noexcept
{
	_shouldCheckValidity = false;
	_parsingInfo->propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return _parsingInfo->propertyParser.getFieldProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return opt::nullopt;
}

//=========================================================================================

CXChildVisitResult FieldParser2::parse(CXCursor const& fieldCursor, ParsingSettings const& parsingSettings, PropertyParser& propertyParser, FieldParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the field parser
	assert(fieldCursor.kind == CXCursorKind::CXCursor_VarDecl || fieldCursor.kind == CXCursorKind::CXCursor_FieldDecl);

	//Init context
	initContext(fieldCursor, parsingSettings, propertyParser, out_result);

	clang_visitChildren(fieldCursor, &FieldParser2::parseEntity, this);

	return (parsingSettings.shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
}

void FieldParser2::initContext(CXCursor const& fieldCursor, ParsingSettings const& parsingSettings, PropertyParser& propertyParser, FieldParsingResult& out_result) noexcept
{
	parsingContext.rootCursor					= fieldCursor;
	parsingContext.shouldCheckEntityValidity	= true;
	parsingContext.propertyParser				= &propertyParser;
	parsingContext.parsingSettings				= &parsingSettings;
	parsingContext.parsingResult				= &out_result;
}

CXChildVisitResult FieldParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	FieldParser2* parser = reinterpret_cast<FieldParser2*>(clientData);

	if (parser->parsingContext.shouldCheckEntityValidity)
	{
		parser->parsingContext.shouldCheckEntityValidity = false;

		//Set parsed field in result if it is valid
		return parser->setParsedEntity(cursor);
	}
	else
	{
		return CXChildVisitResult::CXChildVisit_Continue;
	}
}

CXChildVisitResult FieldParser2::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	FieldParsingResult* result = getParsingResult();

	if (opt::optional<PropertyGroup> propertyGroup = getProperties(annotationCursor))
	{
		FieldInfo& field = result->parsedField.emplace(FieldInfo(parsingContext.rootCursor, std::move(*propertyGroup)));

		//field.accessSpecifier = _parsingInfo->accessSpecifier;
		field.type = TypeInfo(clang_getCursorType(parsingContext.rootCursor));
		field.qualifiers.isStatic = (clang_getCursorKind(parsingContext.rootCursor) == CXCursorKind::CXCursor_VarDecl);

		if (!field.qualifiers.isStatic)
		{
			field.qualifiers.isMutable = clang_CXXField_isMutable(parsingContext.rootCursor);

			field.memoryOffset = clang_Cursor_getOffsetOfField(parsingContext.rootCursor);

			// assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_Invalid);	<- Assert here on travis for some reasons...
			assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_Incomplete);
			assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_Dependent);
			assert(field.memoryOffset != CXTypeLayoutError::CXTypeLayoutError_InvalidFieldName);

			field.memoryOffset /= 8;	//From bits to bytes
		}
	}
	else if (parsingContext.propertyParser->getParsingError() != EParsingError::Count)
	{
		//Fatal parsing error occured
		result->errors.emplace_back(ParsingError(parsingContext.propertyParser->getParsingError(), clang_getCursorLocation(annotationCursor)));
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

opt::optional<PropertyGroup> FieldParser2::getProperties(CXCursor const& cursor) noexcept
{
	parsingContext.propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
				parsingContext.propertyParser->getFieldProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
				opt::nullopt;
}