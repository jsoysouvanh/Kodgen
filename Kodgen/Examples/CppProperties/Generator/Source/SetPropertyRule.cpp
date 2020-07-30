#include "SetPropertyRule.h"

using namespace kodgen;

bool SetPropertyRule::isMainPropSyntaxValid(std::string const& mainProperty, EntityInfo::EType entityType) const noexcept
{
	return mainProperty == "Set" && entityType == EntityInfo::EType::Field;
}

bool SetPropertyRule::isSubPropSyntaxValid(std::string const& subProperty, uint8 /* subPropIndex */) const noexcept
{
	return subProperty == "explicit";
}

bool SetPropertyRule::isPropertyGroupValid(PropertyGroup2 const& propertyGroup, uint8 propertyIndex) const noexcept
{
	//Make sure there is a single Set property
	for (size_t i = 0u; i < propertyGroup.complexProperties.size(); i++)
	{
		if (i != propertyIndex && propertyGroup.complexProperties[i].boundPropertyRule == this)
		{
			//TODO: Property appear at least twice

			return false;
		}
	}

	return true;
}

bool SetPropertyRule::isEntityValid(EntityInfo const& /* entity */, uint8 /* propertyIndex */) const noexcept
{
	//No specific check to perform here
	return true;
}