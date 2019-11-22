#include "Properties/PropertyParser.h"

#include <iostream>	//TO DELETE
#include <cassert>

using namespace refureku;

std::optional<PropertyGroup> PropertyParser::getClassProperties(std::string&& annotateMessage) noexcept
{
	static std::string classAnnotation = "RfrkClass:";

	if (annotateMessage.substr(0, classAnnotation.size()) == classAnnotation)
	{
		if (splitProperties(annotateMessage.substr(classAnnotation.size())))
		{
			return checkAndFillClassPropertyGroup(_splitProps);
		}
	}
	else
	{
		//Tried to add properties to a class with the wrong macro
		_parsingError = EParsingError::WrongPropertyMacroUsed;
	}

	assert(_parsingError != EParsingError::Count);	//If fails, _parsing error must be updated
	return std::nullopt;
}

std::optional<PropertyGroup> PropertyParser::getStructProperties(std::string&& annotateMessage) noexcept
{
	static std::string structAnnotation = "RfrkStruct:";

	return std::nullopt;
}

std::optional<PropertyGroup> PropertyParser::getFieldProperties(std::string&& annotateMessage) noexcept
{
	static std::string fieldAnnotation = "RfrkField:";

	return std::nullopt;
}

std::optional<PropertyGroup> PropertyParser::getMethodProperties(std::string&& annotateMessage) noexcept
{
	static std::string methodAnnotation = "RfrkMethod:";

	return std::nullopt;
}

std::optional<PropertyGroup> PropertyParser::getEnumProperties(std::string&& annotateMessage) noexcept
{
	static std::string enumAnnotation = "RfrkEnum:";

	return std::nullopt;
}

std::optional<PropertyGroup> PropertyParser::getEnumValueProperties(std::string&& annotateMessage) noexcept
{
	static std::string enumAnnotation = "RfrkEnumVal:";

	return std::nullopt;
}

bool PropertyParser::splitProperties(std::string&& propertiesString) noexcept
{
	cleanString(propertiesString);

	bool isParsingSubproperty = false;

	while (!propertiesString.empty())
	{
		if (isParsingSubproperty)
		{
			if (!lookForNextSubProp(propertiesString, isParsingSubproperty))
			{
				return false;
			}
		}
		else if (!lookForNextProp(propertiesString, isParsingSubproperty))
		{
			return false;
		}
	}

	//propertyString has been fully parsed but no end subproperty mark found
	if (isParsingSubproperty)
	{
		_parsingError = EParsingError::SubPropertyEndEncloserMissing;
		return false;
	}

	return true;
}

bool PropertyParser::lookForNextProp(std::string& inout_parsingProps, bool& out_isParsingSubProp)
{
	//Find first occurence of propertySeparator or subprop start encloser in string
	size_t index = inout_parsingProps.find_first_of(_relevantCharsForPropParsing);

	//Was last prop
	if (index == inout_parsingProps.npos)
	{
		_splitProps.push_back({ inout_parsingProps });
		inout_parsingProps.clear();
	}
	else if (inout_parsingProps[index] == _propertyParsingSettings->propertySeparator)
	{
		_splitProps.push_back({ std::string(inout_parsingProps.cbegin(), inout_parsingProps.cbegin() + index) });
		inout_parsingProps.erase(0, index + 1);
	}
	else	//_propertyParsingSettings->subPropertyEnclosers[0]
	{
		out_isParsingSubProp = true;

		_splitProps.push_back({ std::string(inout_parsingProps.cbegin(), inout_parsingProps.cbegin() + index) });
		inout_parsingProps.erase(0, index + 1);
	}

	return true;
}

bool PropertyParser::lookForNextSubProp(std::string& inout_parsingProps, bool& out_isParsingSubProp)
{
	//Find first occurence of propertySeparator or subprop start encloser in string
	size_t index = inout_parsingProps.find_first_of(_relevantCharsForSubPropParsing);

	//Was last prop
	if (index == inout_parsingProps.npos)
	{
		_parsingError = EParsingError::SubPropertyEndEncloserMissing;
		return false;
	}
	else if (inout_parsingProps[index] == _propertyParsingSettings->subPropertySeparator)
	{
		_splitProps.back().push_back(std::string(inout_parsingProps.cbegin(), inout_parsingProps.cbegin() + index));
		inout_parsingProps.erase(0, index + 1);
	}
	else	//_propertyParsingSettings->subPropertyEnclosers[1]
	{
		//Make sure there is a property separator after the end encloser if is not the last char of the string
		if (index != inout_parsingProps.size() - 1 && inout_parsingProps[index + 1] != _propertyParsingSettings->propertySeparator)
		{
			_parsingError = EParsingError::PropertySeparatorMissing;
			return false;
		}

		out_isParsingSubProp = false;

		_splitProps.back().push_back(std::string(inout_parsingProps.cbegin(), inout_parsingProps.cbegin() + index));
		inout_parsingProps.erase(0, index + 2);	// + 2 to consume subprop end encloser and prop separator
	}

	return true;
}

void PropertyParser::cleanString(std::string& toCleanString) const noexcept
{
	for (char toRemoveChar : _propertyParsingSettings->ignoredCharacters)
	{
		toCleanString.erase(std::remove(toCleanString.begin(), toCleanString.end(), toRemoveChar), toCleanString.end());
	}
}

std::optional<PropertyGroup> PropertyParser::checkAndFillClassPropertyGroup(std::vector<std::vector<std::string>>& splitProps) noexcept
{
	PropertyGroup propertyGroup;

	for (std::vector<std::string>& props : splitProps)
	{
		//Expect a simple prop
		if (props.size() == 1)
		{
			if (!addSimpleProperty(props, propertyGroup))
			{
				return std::nullopt;
			}
		}
		//Expect a complex prop
		else if (!addComplexProperty(props, propertyGroup))
		{
			return std::nullopt;
		}
	}

	return propertyGroup;
}

bool PropertyParser::addSimpleProperty(std::vector<std::string>& propertyAsVector, PropertyGroup& out_propertyGroup) noexcept
{
	std::string propName = std::move(propertyAsVector[0]);

	if (_propertyParsingSettings->classPropertyRules.getSimplePropertyRule(propName) != nullptr)
	{
		out_propertyGroup.simpleProperties.emplace(SimpleProperty(std::move(propName)));
		return true;
	}
	else
	{
		_parsingError = EParsingError::InvalidSimpleProperty;
		return false;
	}
}

bool PropertyParser::addComplexProperty(std::vector<std::string>& propertyAsVector, PropertyGroup& out_propertyGroup) noexcept
{
	std::string mainProp = std::move(propertyAsVector[0]);

	if (ComplexPropertyRule const* propertyRule = _propertyParsingSettings->classPropertyRules.getComplexPropertyRule(mainProp))
	{
		ComplexProperty complexProp;
		complexProp.name = std::move(mainProp);

		//Iterate over subproperties
		for (uint8 i = 1u; i < propertyAsVector.size(); i++)
		{
			std::string subProp = std::move(propertyAsVector[i]);

			if (propertyRule->isValidSubProperty(subProp))
			{
				complexProp.subProperties.emplace_back(std::move(subProp));
			}
			else
			{
				_parsingError = EParsingError::InvalidComplexSubProperty;
				return false;
			}
		}

		out_propertyGroup.complexProperties.emplace(std::move(complexProp));
	}
	else
	{
		_parsingError = EParsingError::InvalidComplexMainProperty;
		return false;
	}

	return true;
}

void PropertyParser::setup(PropertyParsingSettings const* propertyParsingSettings) noexcept
{
	_propertyParsingSettings = propertyParsingSettings;

	char charsForPropParsing[] =	{
		_propertyParsingSettings->propertySeparator,
		_propertyParsingSettings->subPropertyEnclosers[0]
	};

	char charsForSubPropParsing[] =	{
		_propertyParsingSettings->subPropertySeparator,
		_propertyParsingSettings->subPropertyEnclosers[1]
	};

	_relevantCharsForPropParsing = std::string(charsForPropParsing, 2);
	_relevantCharsForSubPropParsing = std::string(charsForSubPropParsing, 2);
}

void PropertyParser::clean() noexcept
{
	_splitProps.clear();
	_parsingError = EParsingError::Count;
}

EParsingError PropertyParser::getParsingError() const noexcept
{
	return _parsingError;
}