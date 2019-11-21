#include "InfoStructures/MethodInfo.h"

using namespace refureku;

MethodInfo::MethodInfo(std::string&& entityName, PropertyGroup&& propertyGroup) noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
}