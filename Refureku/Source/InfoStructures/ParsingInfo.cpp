#include "InfoStructures/ParsingInfo.h"

#include <iostream>	//TO DELETE

#include "Helpers.h"

using namespace refureku;

void ParsingInfo::startStructParsing(CXCursor const& structCursor) noexcept
{
	_classStructLevel++;
	_currentClassCursor = structCursor;
	shouldCheckValidity = true;
	_accessSpecifier = EAccessSpecifier::Public;

	std::cout << "START STRUCT" << std::endl;
}

void ParsingInfo::startClassParsing(CXCursor const& classCursor) noexcept
{
	_classStructLevel++;
	_currentClassCursor = classCursor;
	shouldCheckValidity = true;
	_accessSpecifier = EAccessSpecifier::Private;

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
	_accessSpecifier = static_cast<EAccessSpecifier>(1 << clang_getCXXAccessSpecifier(enumCursor));
}

void ParsingInfo::addParsingError(EParsingError parsingError) noexcept
{
	_parsingResult.parsingErrors.emplace_back(ParsingError(parsingError));
}

CXChildVisitResult ParsingInfo::tryToAddClass(CXCursor const& classAnnotationCursor) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isClassValid(classAnnotationCursor))
	{
		_parsingResult.classes.emplace_back(ClassInfo(Helpers::getString(clang_getCursorDisplayName(_currentClassCursor)), std::move(*propertyGroup)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (_propertyParser.getParsingError() == EParsingError::Count)
		{
			endStructOrClassParsing();
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			_parsingResult.parsingErrors.emplace_back(ParsingError(_propertyParser.getParsingError(), clang_getCursorLocation(classAnnotationCursor)));

			return _parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

std::optional<PropertyGroup> ParsingInfo::isClassValid(CXCursor currentCursor) noexcept
{
	shouldCheckValidity = false;
	_propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return _propertyParser.getClassProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

bool ParsingInfo::hasErrorOccured() const noexcept
{
	return !_parsingResult.parsingErrors.empty();
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

EAccessSpecifier ParsingInfo::getAccessSpecifier() const noexcept
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