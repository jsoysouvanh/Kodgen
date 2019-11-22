#include "InfoStructures/EntityInfo.h"

using namespace refureku;

EntityInfo::EntityInfo(std::string&& entityName, PropertyGroup&& propertyGroup) noexcept:
	name{std::forward<std::string>(entityName)}, 
	properties{std::forward<PropertyGroup>(propertyGroup)}
{
}

std::ostream& refureku::operator<<(std::ostream& out_stream, EntityInfo const& entityInfo) noexcept
{
	out_stream << "EntityInfo";

	return out_stream;
}