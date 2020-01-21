#include "InfoStructures/EnumInfo.h"

using namespace refureku;

EnumInfo::EnumInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup), EType::Enum)
{
}