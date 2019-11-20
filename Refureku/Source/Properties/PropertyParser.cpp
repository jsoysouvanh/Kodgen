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
			//TODO
		}
		else
		{
			//TODO
		}
	}

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
	//_splitProps

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
		while ((it = std::find_if(propertiesString.begin(), propertiesString.end(), [this](char c){ return c == propertySeparator; })) != propertiesString.end())
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

		size_t charIndex = fullProp.find_first_of(subPropertyEnclosers[0], 0u);
		if (charIndex != fullProp.npos)	//found subprops beginning marker
		{
			//Make sure the last char is a subprops ending marker
			if (fullProp.back() != subPropertyEnclosers[1])
			{
				_errorMessage = "Couldn't find subproperty ending marker in " + fullProp;
				return false;
			}

			subProps = fullProp.substr(charIndex + 1, fullProp.size() - charIndex - 2);
			fullProp.erase(charIndex);

			//Split sub properties
			std::string::iterator it;

			//Split main properties first
			while ((it = std::find_if(subProps.begin(), subProps.end(), [this](char c){ return c == subPropertySeparator; })) != subProps.end())
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
	for (char toRemoveChar : ignoredCharacters)
	{
		toCleanString.erase(std::remove(toCleanString.begin(), toCleanString.end(), toRemoveChar), toCleanString.end());
	}
}

void PropertyParser::setup() noexcept
{
	_hasCommonSeparator = propertySeparator == subPropertySeparator;
}

std::string const& PropertyParser::getErrorMessage() const noexcept
{
	return _errorMessage;
}