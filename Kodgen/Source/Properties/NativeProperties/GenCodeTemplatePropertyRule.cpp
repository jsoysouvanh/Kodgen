#include "Properties/NativeProperties/GenCodeTemplatePropertyRule.h"

#include "Properties/NativeProperties/NativeProperties.h"

using namespace kodgen;

bool GenCodeTemplatePropertyRule::isMainPropSyntaxValid(std::string const& mainProperty, EntityInfo::EType entityType) const noexcept
{
	return mainProperty == NativeProperties::generatedCodeTemplateProperty;
}

bool GenCodeTemplatePropertyRule::isSubPropSyntaxValid(std::string const& subProperty, uint8 subPropIndex) const noexcept
{
	return validTemplateNames.find(subProperty) != validTemplateNames.cend();
}

bool GenCodeTemplatePropertyRule::isPropertyGroupValid(PropertyGroup2 const& propertyGroup, uint8 propertyIndex) const noexcept
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

	//Check that there is a single parameter
	if (propertyGroup.complexProperties[propertyIndex].subProperties.size() != 1u)
	{
		//TODO: Issue error: This property should have a single sub property

		return false;
	}

	return true;
}

bool GenCodeTemplatePropertyRule::isEntityValid(EntityInfo const& entity, uint8 propertyIndex) const noexcept
{
	//Nothing special to check
	return true;
}