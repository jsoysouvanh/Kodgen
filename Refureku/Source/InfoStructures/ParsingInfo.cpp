#include "InfoStructures/ParsingInfo.h"

#include <cassert>
#include <iostream>	//TO DELETE

#include "Helpers.h"

using namespace refureku;

void ParsingInfo::startEnumParsing(CXCursor const& enumCursor) noexcept
{
	_isParsingEnum = true;
	_currentEnumCursor = enumCursor;
	shouldCheckValidity = true;
	//std::cout << "START ENUM" << std::endl;
}

void ParsingInfo::endEnumParsing() noexcept
{
	_isParsingEnum = false;
	_currentEnumCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	//std::cout << "END ENUM" << std::endl;
}

void ParsingInfo::addParsingError(EParsingError parsingError) noexcept
{
	_parsingResult.parsingErrors.emplace_back(ParsingError(parsingError));
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

void ParsingInfo::setParsingSettings(ParsingSettings const* parsingSettings) noexcept
{
	_parsingSettings = parsingSettings;
	_propertyParser.setup(&parsingSettings->propertyParsingSettings);
}

ParsingSettings const* ParsingInfo::getParsingSettings() const noexcept
{
	return _parsingSettings;
}