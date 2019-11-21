#include "InfoStructures/EntityInfo.h"

using namespace refureku;

EntityInfo::EntityInfo(std::string&& entityName, PropertyGroup&& propertyGroup) noexcept:
	name{std::forward<std::string>(entityName)}, 
	properties{std::forward<PropertyGroup>(propertyGroup)}
{
}