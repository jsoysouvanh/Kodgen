#include "Properties/NativeProperties/ParseAllNestedPropertyRule.h"

#include "Properties/NativeProperties/NativeProperties.h"

using namespace kodgen;

bool ParseAllNestedPropertyRule::isMainPropSyntaxValid(std::string const& mainProperty, EntityInfo::EType entityType) const noexcept
{
	return mainProperty == NativeProperties::parseAllNestedProperty &&
		(entityType == EntityInfo::EType::Namespace || entityType == EntityInfo::EType::Class || entityType == EntityInfo::EType::Struct);
}

bool ParseAllNestedPropertyRule::isPropertyGroupValid(PropertyGroup2 const& propertyGroup, uint8 propertyIndex) const noexcept
{
	//Check that this property doesn't appear twice
	for (uint8 i = 0u; i < propertyGroup.simpleProperties.size(); i++)
	{
		if (i != propertyIndex && propertyGroup.simpleProperties[i].boundPropertyRule == this)
		{
			//TODO: Issue error: This property appeared twice

			return false;
		}
	}

	return true;
}

bool ParseAllNestedPropertyRule::isEntityValid(EntityInfo const& /* entity */, uint8 /* propertyIndex */) const noexcept
{
	//Nothing special to check
	return true;
}