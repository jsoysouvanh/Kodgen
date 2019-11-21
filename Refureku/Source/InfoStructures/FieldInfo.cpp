#include "InfoStructures/FieldInfo.h"

using namespace refureku;

FieldInfo::FieldInfo(std::string&& entityName, PropertyGroup&& propertyGroup) noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
}