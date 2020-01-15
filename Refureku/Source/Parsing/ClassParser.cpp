#include "Parsing/ClassParser.h"

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
		return tryToAddClass(currentCursor, parsingInfo);
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

CXChildVisitResult ClassParser::tryToAddClass(CXCursor classAnnotationCursor, ParsingInfo& parsingInfo) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isClassValid(classAnnotationCursor, parsingInfo))
	{
		parsingInfo.currentClass.emplace(ClassInfo(Helpers::getString(clang_getCursorDisplayName(_currentCursor)), std::move(*propertyGroup)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (parsingInfo.propertyParser.getParsingError() == EParsingError::Count)
		{
			endStructOrClassParsing(parsingInfo);
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(parsingInfo.propertyParser.getParsingError(), clang_getCursorLocation(classAnnotationCursor)));
			parsingInfo.currentClass.reset();

			return parsingInfo.parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

std::optional<PropertyGroup> ClassParser::isClassValid(CXCursor currentCursor, ParsingInfo& parsingInfo) noexcept
{
	_shouldCheckValidity = false;
	parsingInfo.propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo.propertyParser.getClassProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

void ClassParser::startClassParsing(CXCursor currentCursor, ParsingInfo& parsingInfo)	noexcept
{
	_classLevel++;
	_currentCursor					= currentCursor;
	_shouldCheckValidity			= true;
	parsingInfo.accessSpecifier		= EAccessSpecifier::Private;

	//std::cout << "START CLASS" << std::endl;
}

void ClassParser::startStructParsing(CXCursor currentCursor, ParsingInfo& parsingInfo)	noexcept
{
	_classLevel++;
	_currentCursor					= currentCursor;
	_shouldCheckValidity			= true;
	parsingInfo.accessSpecifier		= EAccessSpecifier::Public;

	//std::cout << "START STRUCT" << std::endl;
}

void ClassParser::endStructOrClassParsing(ParsingInfo& parsingInfo)	noexcept
{
	_classLevel--;
	_currentCursor			= clang_getNullCursor();
	_shouldCheckValidity	= false;

	if (parsingInfo.currentClass.has_value())
	{
		parsingInfo.parsingResult.classes.emplace_back(std::move(parsingInfo.currentClass.value()));
		parsingInfo.currentClass.reset();
	}

	//std::cout << "END CLASS/STRUCT" << std::endl;
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
		endStructOrClassParsing(parsingInfo);
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