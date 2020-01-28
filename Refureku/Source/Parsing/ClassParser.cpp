#include "Parsing/ClassParser.h"

#include <cassert>

#include "Misc/Helpers.h"

using namespace refureku;

CXChildVisitResult ClassParser::parse(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	if (_fieldParser.isCurrentlyParsing())
	{
		return _fieldParser.parse(currentCursor, parsingInfo);
	}
	else if (_methodParser.isCurrentlyParsing())
	{
		return _methodParser.parse(currentCursor, parsingInfo);
	}
	else if (_shouldCheckValidity)	//Check for any annotation attribute if the flag is raised
	{
		_shouldCheckValidity = false;
		return setAsCurrentStructOrClassIfValid(currentCursor, parsingInfo);
	}

	//Check for class field or method
	switch (clang_getCursorKind(currentCursor))
	{
		case CXCursorKind::CXCursor_CXXAccessSpecifier:
			updateAccessSpecifier(currentCursor, parsingInfo);
			break;

		case CXCursorKind::CXCursor_FieldDecl:
			_fieldParser.startParsing(currentCursor);
			break;

		case CXCursorKind::CXCursor_CXXMethod:
			_methodParser.startParsing(currentCursor);
			break;

		default:
			return CXChildVisitResult::CXChildVisit_Continue;
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult ClassParser::setAsCurrentStructOrClassIfValid(CXCursor classAnnotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (opt::optional<PropertyGroup> propertyGroup = isStructOrClassValid(classAnnotationCursor, parsingInfo))
	{
		parsingInfo.currentStructOrClass.emplace(StructClassInfo(Helpers::getString(clang_getCursorDisplayName(_currentCursor)), std::move(*propertyGroup), std::move(_structOrClass)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (parsingInfo.propertyParser.getParsingError() == EParsingError::Count)
		{
			endParsing(parsingInfo);
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo.propertyParser.getParsingError(), clang_getCursorLocation(classAnnotationCursor)));

			return parsingInfo.parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

opt::optional<PropertyGroup> ClassParser::isStructOrClassValid(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	parsingInfo.propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		switch (_structOrClass)
		{
			case EntityInfo::EType::Class:
				return parsingInfo.propertyParser.getClassProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
				break;

			case EntityInfo::EType::Struct:
				return parsingInfo.propertyParser.getStructProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
				break;

			default:
				assert(false);	//Should never pass here
		}
	}

	return opt::nullopt;
}

void ClassParser::startClassParsing(CXCursor currentCursor, ParsingInfo& parsingInfo)	noexcept
{
	_classLevel++;
	_currentCursor					= currentCursor;
	_shouldCheckValidity			= true;
	parsingInfo.accessSpecifier		= EAccessSpecifier::Private;
	_structOrClass					= EntityInfo::EType::Class;
}

void ClassParser::startStructParsing(CXCursor currentCursor, ParsingInfo& parsingInfo)	noexcept
{
	_classLevel++;
	_currentCursor					= currentCursor;
	_shouldCheckValidity			= true;
	parsingInfo.accessSpecifier		= EAccessSpecifier::Public;
	_structOrClass					= EntityInfo::EType::Struct;
}

void ClassParser::endParsing(ParsingInfo& parsingInfo)	noexcept
{
	_classLevel--;
	_currentCursor					= clang_getNullCursor();
	_shouldCheckValidity			= false;
	parsingInfo.accessSpecifier		= EAccessSpecifier::Invalid;
	_structOrClass					= EntityInfo::EType::Count;

	parsingInfo.flushCurrentStructOrClass();
}

void ClassParser::updateParsingState(CXCursor parent, ParsingInfo& parsingInfo) noexcept
{
	//Check if we're not parsing a field anymore
	if (_fieldParser.isCurrentlyParsing())
	{
		_fieldParser.updateParsingState(parent);
	}
	//Check if we're not parsing a method anymore
	else if (_methodParser.isCurrentlyParsing())
	{
		_methodParser.updateParsingState(parent);
	}

	/**
	*	Check if we're finishing parsing a class
	*/
	if (clang_equalCursors(clang_getCursorSemanticParent(_currentCursor), parent))
	{
		endParsing(parsingInfo);
	}
}

void ClassParser::updateAccessSpecifier(CXCursor cursor, ParsingInfo& parsingInfo) noexcept
{
	parsingInfo.accessSpecifier = static_cast<EAccessSpecifier>(1 << clang_getCXXAccessSpecifier(cursor));
}

bool ClassParser::isCurrentlyParsing() const noexcept
{
	return _classLevel;
}