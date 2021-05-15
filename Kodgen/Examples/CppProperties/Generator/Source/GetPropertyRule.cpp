#include "GetPropertyRule.h"

#include <algorithm>

using namespace kodgen;

GetPropertyRule::GetPropertyRule() noexcept:
	DefaultPropertyRule("Get", EEntityType::Field)
{
}

bool GetPropertyRule::isValid(EntityInfo const& entity, uint8 propertyIndex) const noexcept
{
	if (!DefaultPropertyRule::isValid(entity, propertyIndex))
	{
		return false;
	}

	Property const& prop = entity.propertyGroup.properties[propertyIndex];

	//Check that arguments are valid
	for (std::string const& arg : prop.arguments)
	{
		if (arg != "*" && arg != "&" && arg != "const" && arg != "explicit")
		{
			return false;
		}
	}

	//Can't have * and & at the same time
	if (std::find(prop.arguments.cbegin(), prop.arguments.cend(), "*") != prop.arguments.cend() &&
		std::find(prop.arguments.cbegin(), prop.arguments.cend(), "&") != prop.arguments.cend())
	{
		return false;
	}

	//Make sure no previous property was already valid for this rule
	for (int i = 0; i < propertyIndex; i++)
	{
		if (isValid(entity, i))
		{
			return false;
		}
	}

	return true;
}