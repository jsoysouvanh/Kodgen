#include "SetPropertyRule.h"

using namespace kodgen;

SetPropertyRule::SetPropertyRule() noexcept:
	DefaultPropertyRule("Set", EEntityType::Field)
{
}

bool SetPropertyRule::isValid(EntityInfo const& entity, uint8 propertyIndex) const noexcept
{
	if (!DefaultPropertyRule::isValid(entity, propertyIndex))
	{
		return false;
	}

	Property const& prop = entity.propertyGroup.properties[propertyIndex];

	//Check that there is at most 1 argument and the only valid one is 'explicit'
	if (prop.arguments.size() > 1 || (prop.arguments.size() > 0 && prop.arguments[0] != "explicit"))
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