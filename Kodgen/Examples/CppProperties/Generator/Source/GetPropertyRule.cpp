#include "GetPropertyRule.h"

#include <algorithm>

using namespace kodgen;

bool GetPropertyRule::isMainPropSyntaxValid(std::string const& mainProperty, EntityInfo::EType entityType) const noexcept
{
	return mainProperty == "Get" && entityType == EntityInfo::EType::Field;
}

bool GetPropertyRule::isSubPropSyntaxValid(std::string const& subProperty, uint8 /* subPropIndex */) const noexcept
{
	return	subProperty == "*"			||
			subProperty == "&"			||
			subProperty == "const"		||
			subProperty == "explicit";
}

bool GetPropertyRule::isPropertyGroupValid(PropertyGroup2 const& propertyGroup, uint8 propertyIndex) const noexcept
{
	//Make sure there is a single Get property
	for (size_t i = 0u; i < propertyGroup.complexProperties.size(); i++)
	{
		if (i != propertyIndex && propertyGroup.complexProperties[i].boundPropertyRule == this)
		{
			//TODO: Property appear at least twice

			return false;
		}
	}

	ComplexProperty2 const& complexProp = propertyGroup.complexProperties[propertyIndex];

	//Can't have * and & at the same time
	if (std::find(complexProp.subProperties.cbegin(), complexProp.subProperties.cend(), "*") != complexProp.subProperties.cend() &&
		std::find(complexProp.subProperties.cbegin(), complexProp.subProperties.cend(), "&") != complexProp.subProperties.cend())
	{
		//TODO: Can't have * and & at the same time

		return false;
	}

	return true;
}

bool GetPropertyRule::isEntityValid(EntityInfo const& /* entity */, uint8 /* propertyIndex */) const noexcept
{
	//No specific check to perform here
	return true;
}