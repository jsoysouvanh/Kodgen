#include "InfoStructures/ClassInfo.h"

using namespace refureku;

ClassInfo::ClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept:
	StructClassInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
}