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

	if (_hasCommonSeparator)
	{
		//TODO
		return true;
	}
	else
	{
		std::string::iterator it;

		//Split main properties first
		while ((it = std::find_if(propertiesString.begin(), propertiesString.end(), [this](char c){ return c == _propertyParsingSettings->propertySeparator; })) != propertiesString.end())
		{
			_splitProps.push_back({ std::string(propertiesString.begin(), it) });
			propertiesString.erase(propertiesString.cbegin(), it + 1);
		}

		//Retrieve last one
		_splitProps.push_back({ std::string(propertiesString.begin(), it) });
	}

	return splitSubProperties(_splitProps);
}

bool PropertyParser::splitSubProperties(std::vector<std::vector<std::string>>& splitProps) noexcept
{
	std::string subProps;

	for (std::vector<std::string>& vec : splitProps)
	{
		std::string& fullProp = vec.front();

		size_t charIndex = fullProp.find_first_of(_propertyParsingSettings->subPropertyEnclosers[0], 0u);
		if (charIndex != fullProp.npos)	//found subprops beginning marker
		{
			//Make sure the last char is a subprops ending marker
			if (fullProp.back() != _propertyParsingSettings->subPropertyEnclosers[1])
			{
				_parsingError = EParsingError::SubPropertyEndEncloserMissing;
				return false;
			}

			subProps = fullProp.substr(charIndex + 1, fullProp.size() - charIndex - 2);
			fullProp.erase(charIndex);

			//Split sub properties
			std::string::iterator it;

			//Split main properties first
			while ((it = std::find_if(subProps.begin(), subProps.end(), [this](char c){ return c == _propertyParsingSettings->subPropertySeparator; })) != subProps.end())
			{
				vec.push_back({ std::string(subProps.begin(), it) });
				subProps.erase(subProps.cbegin(), it + 1);
			}

			//Retrieve last one
			vec.push_back({ std::string(subProps.begin(), it) });
		}
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
			std::string propName = std::move(props[0]);

			if (_propertyParsingSettings->classPropertyRules.getSimplePropertyRule(propName) != nullptr)
			{
				propertyGroup.simpleProperties.emplace(SimpleProperty(std::move(propName)));
			}
			else
			{
				_parsingError = EParsingError::InvalidSimpleProperty;
				return std::nullopt;
			}
		}
		//Expect a complex prop
		else
		{
			std::string mainProp = std::move(props[0]);

			if (ComplexPropertyRule const* propertyRule = _propertyParsingSettings->classPropertyRules.getComplexPropertyRule(mainProp))
			{
				ComplexProperty complexProp;
				complexProp.name = std::move(mainProp);

				//Iterate over subproperties
				for (uint8 i = 1u; i < props.size(); i++)
				{
					std::string subProp = std::move(props[i]);

					if (propertyRule->isValidSubProperty(subProp))
					{
						complexProp.subProperties.emplace_back(std::move(subProp));
					}
					else
					{
						_parsingError = EParsingError::InvalidComplexSubProperty;
						return std::nullopt;
					}
				}

				propertyGroup.complexProperties.emplace(std::move(complexProp));
			}
			else
			{
				_parsingError = EParsingError::InvalidComplexMainProperty;
				return std::nullopt;
			}
		}
	}

	return propertyGroup;
}

void PropertyParser::setup(PropertyParsingSettings const* propertyParsingSettings) noexcept
{
	_propertyParsingSettings = propertyParsingSettings;
	_hasCommonSeparator = _propertyParsingSettings->propertySeparator == _propertyParsingSettings->subPropertySeparator;
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