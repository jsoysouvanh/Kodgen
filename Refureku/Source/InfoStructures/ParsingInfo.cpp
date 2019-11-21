#include "InfoStructures/ParsingInfo.h"

#include <iostream>	//TO DELETE

#include "Helpers.h"

using namespace refureku;

void ParsingInfo::startStructParsing(CXCursor const& structCursor) noexcept
{
	_classStructLevel++;
	_currentClassCursor = structCursor;
	shouldCheckValidity = true;
	_accessSpecifier = AccessSpecifier::Public;

	std::cout << "START STRUCT" << std::endl;
}

void ParsingInfo::startClassParsing(CXCursor const& classCursor) noexcept
{
	_classStructLevel++;
	_currentClassCursor = classCursor;
	shouldCheckValidity = true;
	_accessSpecifier = AccessSpecifier::Private;

	std::cout << "START CLASS" << std::endl;
}

void ParsingInfo::startFieldParsing(CXCursor const& fieldCursor) noexcept
{
	_isParsingField = true;
	_currentFieldCursor = fieldCursor;
	shouldCheckValidity = true;
	std::cout << "START FIELD" << std::endl;
}

void ParsingInfo::startMethodParsing(CXCursor const& methodCursor) noexcept
{
	_isParsingMethod = true;
	_currentMethodCursor = methodCursor;
	shouldCheckValidity = true;
	std::cout << "START METHOD" << std::endl;
}

void ParsingInfo::startEnumParsing(CXCursor const& enumCursor) noexcept
{
	_isParsingEnum = true;
	_currentEnumCursor = enumCursor;
	shouldCheckValidity = true;
	std::cout << "START ENUM" << std::endl;
}

void ParsingInfo::endStructOrClassParsing() noexcept
{
	_classStructLevel--;
	_currentClassCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END CLASS/STRUCT" << std::endl;
}

void ParsingInfo::endFieldParsing() noexcept
{
	_isParsingField = false;
	_currentFieldCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END FIELD" << std::endl;
}

void ParsingInfo::endMethodParsing() noexcept
{
	_isParsingMethod = false;
	_currentMethodCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END METHOD" << std::endl;
}

void ParsingInfo::endEnumParsing() noexcept
{
	_isParsingEnum = false;
	_currentEnumCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END ENUM" << std::endl;
}

void ParsingInfo::updateAccessSpecifier(CXCursor const& enumCursor) noexcept
{
	_accessSpecifier = static_cast<AccessSpecifier>(1 << clang_getCXXAccessSpecifier(enumCursor));

	//TODO delete this
	switch (_accessSpecifier)
	{
		case AccessSpecifier::Private:
			std::cout << " -- Private:" << std::endl;
			break;
		case AccessSpecifier::Protected:
			std::cout << " -- Protected:" << std::endl;
			break;
		case AccessSpecifier::Public:
			std::cout << " -- Public:" << std::endl;
			break;
		default:
			std::cout << " -- Invalid:" << std::endl;
			break;
	}
}

CXChildVisitResult ParsingInfo::tryToAddClass(CXCursor const& classAnnotationCursor) noexcept
{
	if (isClassValid(classAnnotationCursor))
	{
		//TODO create a class

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (_propertyParser.getParsingError() == PropertyParsingError::Count)
		{
			endStructOrClassParsing();
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			_parsingResult.parsingErrors.emplace_back(ParsingError(clang_getCursorLocation(classAnnotationCursor), _propertyParser.getParsingError()));

			return _parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}

	return CXChildVisitResult::CXChildVisit_Recurse;//TODO
}

bool ParsingInfo::isClassValid(CXCursor currentCursor) noexcept
{
	//std::string		cursorName	= ParsingInfo::getString(clang_getCursorSpelling(currentCursor));

	shouldCheckValidity = false;

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return _propertyParser.getClassProperties(Helpers::getString(clang_getCursorSpelling(currentCursor))).has_value();
	}

	return false;
}

ParsingResult ParsingInfo::extractParsingResult() noexcept
{
	return std::move(_parsingResult);
}

uint8 ParsingInfo::getClassStructLevel() const noexcept
{
	return _classStructLevel;
}

CXCursor const& ParsingInfo::getCurrentClassCursor() const noexcept
{
	return _currentClassCursor;
}

bool ParsingInfo::isParsingEnum() const noexcept
{
	return _isParsingEnum;
}

bool ParsingInfo::isParsingField() const noexcept
{
	return _isParsingField;
}

bool ParsingInfo::isParsingMethod() const noexcept
{
	return _isParsingMethod;
}

CXCursor const& ParsingInfo::getCurrentEnumCursor()const noexcept
{
	return _currentEnumCursor;
}

CXCursor const& ParsingInfo::getCurrentFieldCursor()const noexcept
{
	return _currentFieldCursor;
}

CXCursor const& ParsingInfo::getCurrentMethodCursor()const noexcept
{
	return _currentMethodCursor;
}

AccessSpecifier ParsingInfo::getAccessSpecifier() const noexcept
{
	return _accessSpecifier;
}

void ParsingInfo::setParsingSettings(ParsingSettings const* parsingSettings) noexcept
{
	_parsingSettings = parsingSettings;
	_propertyParser.setup(&parsingSettings->propertyParsingSettings);
}

ParsingSettings const* ParsingInfo::getParsingSettings() const noexcept
{
	return _parsingSettings;
}

//PropertyParser& ParsingInfo::getPropertyParser() noexcept
//{
//	return _propertyParser;
//}