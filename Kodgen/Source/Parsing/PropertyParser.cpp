#include "Kodgen/Parsing/PropertyParser.h"

#include <cassert>

#include "Kodgen/Properties/Property.h"

using namespace kodgen;

opt::optional<PropertyGroup> PropertyParser::getProperties(std::string&& annotateMessage, std::string const& annotationId) noexcept
{
	if (annotateMessage.substr(0, annotationId.size()) == annotationId)
	{
		if (splitProperties(annotateMessage.substr(annotationId.size())))
		{
			return fillPropertyGroup(_splitProps);
		}
	}
	else
	{
		_parsingErrorDescription = "The wrong macro has been used to attach properties to an entity.";
	}

	assert(!_parsingErrorDescription.empty());	//If fails, _parsingErrorDescription must be updated
	return opt::nullopt;
}

opt::optional<PropertyGroup> PropertyParser::getNamespaceProperties(std::string annotateMessage) noexcept
{
	static std::string namespaceAnnotation = "KGN:";

	return getProperties(std::move(annotateMessage), namespaceAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getClassProperties(std::string annotateMessage) noexcept
{
	static std::string classAnnotation = "KGC:";

	return getProperties(std::move(annotateMessage), classAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getStructProperties(std::string annotateMessage) noexcept
{
	static std::string structAnnotation = "KGS:";

	return getProperties(std::move(annotateMessage), structAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getVariableProperties(std::string annotateMessage) noexcept
{
	static std::string variableAnnotation = "KGV:";

	return getProperties(std::move(annotateMessage), variableAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getFieldProperties(std::string annotateMessage) noexcept
{
	static std::string fieldAnnotation = "KGF:";

	return getProperties(std::move(annotateMessage), fieldAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getFunctionProperties(std::string annotateMessage) noexcept
{
	static std::string functionAnnotation = "KGFu:";

	return getProperties(std::move(annotateMessage), functionAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getMethodProperties(std::string annotateMessage) noexcept
{
	static std::string methodAnnotation = "KGM:";

	return getProperties(std::move(annotateMessage), methodAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getEnumProperties(std::string annotateMessage) noexcept
{
	static std::string enumAnnotation = "KGE:";

	return getProperties(std::move(annotateMessage), enumAnnotation);
}

opt::optional<PropertyGroup> PropertyParser::getEnumValueProperties(std::string annotateMessage) noexcept
{
	static std::string enumValueAnnotation = "KGEV:";

	return getProperties(std::move(annotateMessage), enumValueAnnotation);
}

bool PropertyParser::splitProperties(std::string&& propertiesString) noexcept
{
	bool isParsingArgument = false;

	while (!propertiesString.empty())
	{
		if (isParsingArgument)
		{
			if (!lookForNextPropertyArgument(propertiesString, isParsingArgument))
			{
				return false;
			}
		}
		else if (!lookForNextProperty(propertiesString, isParsingArgument))
		{
			return false;
		}
	}

	//propertyString has been fully parsed but no end subproperty mark found
	if (isParsingArgument)
	{
		_parsingErrorDescription = "Subproperty end encloser \"" + std::string(1u, _propertyParsingSettings->argumentEnclosers[1]) + "\" is missing.";
		
		return false;
	}

	return true;
}

bool PropertyParser::lookForNextProperty(std::string& inout_parsingProps, bool& out_isParsingSubProp) noexcept
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

	//Remove start and trail spaces from the added property
	removeStartSpaces(_splitProps.back().front());
	removeTrailSpaces(_splitProps.back().front());

	return true;
}

bool PropertyParser::lookForNextPropertyArgument(std::string& inout_parsingProps, bool& out_isParsingSubProp) noexcept
{
	//Find first occurence of propertySeparator or subprop start encloser in string
	size_t index = inout_parsingProps.find_first_of(_relevantCharsForPropArgsParsing);

	//Was last prop
	if (index == inout_parsingProps.npos)
	{
		_parsingErrorDescription = "Subproperty end encloser \"" + std::string(1u, _propertyParsingSettings->argumentEnclosers[1]) + "\" is missing.";

		return false;
	}
	else if (inout_parsingProps[index] == _propertyParsingSettings->argumentSeparator)
	{
		_splitProps.back().push_back(std::string(inout_parsingProps.cbegin(), inout_parsingProps.cbegin() + index));

		//Remove start and trail spaces from the added sub property
		removeStartSpaces(_splitProps.back().back());
		removeTrailSpaces(_splitProps.back().back());

		inout_parsingProps.erase(0, index + 1);
	}
	else	//_propertyParsingSettings->subPropertyEnclosers[1]
	{
		out_isParsingSubProp = false;

		_splitProps.back().push_back(std::string(inout_parsingProps.cbegin(), inout_parsingProps.cbegin() + index));

		//Remove start and trail spaces from the added sub property
		removeStartSpaces(_splitProps.back().back());
		removeTrailSpaces(_splitProps.back().back());

		//Make sure there is a property separator after the end encloser if is not the last char of the string
		if (index != inout_parsingProps.size() - 1)
		{
			size_t propSeparatorIndex = inout_parsingProps.find_first_not_of(' ', index + 1);

			if (inout_parsingProps[propSeparatorIndex] != _propertyParsingSettings->propertySeparator)
			{
				_parsingErrorDescription = "Property separator \"" + std::string(1, _propertyParsingSettings->propertySeparator) + "\" is missing between two properties.";

				return false;
			}
			else
			{
				inout_parsingProps.erase(0, propSeparatorIndex + 1); // + 1 to consume prop separator
			}
		}
		else
		{
			//index is the last character, so consume the rest of the string
			inout_parsingProps.clear();
		}
	}

	return true;
}

void PropertyParser::removeStartSpaces(std::string& toCleanString) const noexcept
{
	toCleanString.erase(0, toCleanString.find_first_not_of(' '));
}

void PropertyParser::removeTrailSpaces(std::string& toCleanString) const noexcept
{
	toCleanString.erase(toCleanString.find_last_not_of(' ') + 1);
}

PropertyGroup PropertyParser::fillPropertyGroup(std::vector<std::vector<std::string>>& splitProps) noexcept
{
	PropertyGroup propertyGroup;

	for (std::vector<std::string>& props : splitProps)
	{
		addProperty(props, propertyGroup);
	}

	return propertyGroup;
}

void PropertyParser::addProperty(std::vector<std::string>& propertyAsVector, PropertyGroup& out_propertyGroup) noexcept
{
	Property prop{std::move(propertyAsVector[0]), std::vector<std::string>()};

	if (propertyAsVector.size() > 1u)
	{
		//Add all arguments to the property
		//If propertyAsVector has 2 elements and the 2nd element is empty, there are no arguments
		if (propertyAsVector.size() != 2 || !propertyAsVector[1].empty())
			prop.arguments.insert(prop.arguments.cend(), std::make_move_iterator(propertyAsVector.begin() + 1), std::make_move_iterator(propertyAsVector.end()));
	}
	
	out_propertyGroup.properties.emplace_back(std::move(prop));
}

void PropertyParser::setup(PropertyParsingSettings const& propertyParsingSettings) noexcept
{
	_propertyParsingSettings			= &propertyParsingSettings;

	char charsForPropParsing[] =
	{
		_propertyParsingSettings->propertySeparator,
		_propertyParsingSettings->argumentEnclosers[0]
	};

	char charsForSubPropParsing[] =
	{
		_propertyParsingSettings->argumentSeparator,
		_propertyParsingSettings->argumentEnclosers[1]
	};

	_relevantCharsForPropParsing		= std::string(charsForPropParsing, 2);
	_relevantCharsForPropArgsParsing	= std::string(charsForSubPropParsing, 2);
}

void PropertyParser::clean() noexcept
{
	_splitProps.clear();
	_parsingErrorDescription.clear();
}

std::string const& PropertyParser::getParsingErrorDescription() const noexcept
{
	return _parsingErrorDescription;
}