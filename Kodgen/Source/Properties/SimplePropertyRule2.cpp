#include "Properties/SimplePropertyRule2.h"

using namespace kodgen;

bool SimplePropertyRule2::isUsedOnlyOnce(PropertyGroup2 const& propertyGroup, uint8 propertyIndex, std::string& out_errorDescription) const noexcept
{
	for (uint8 i = 0u; i < propertyGroup.simpleProperties.size(); i++)
	{
		if (i != propertyIndex && propertyGroup.simpleProperties[i].boundPropertyRule == this)
		{
			out_errorDescription = "The property \"" + propertyGroup.simpleProperties[i].mainProperty + "\" is used more than once in the property group.";

			return false;
		}
	}

	return true;
}