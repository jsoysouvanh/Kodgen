#include "InfoStructures/StructClassInfo.h"

using namespace refureku;

StructClassInfo::StructClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
}

void StructClassInfo::AddField(EAccessSpecifier accessSpecifier, CXCursor fieldCursor) noexcept
{
	//TODO
}

void StructClassInfo::AddMethod(EAccessSpecifier accessSpecifier, CXCursor methodCursor) noexcept
{
	//TODO
}