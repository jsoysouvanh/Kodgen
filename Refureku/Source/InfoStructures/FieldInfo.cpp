#include "InfoStructures/FieldInfo.h"

using namespace refureku;

FieldInfo::FieldInfo(std::string&& entityName, PropertyGroup&& propertyGroup) noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup), EType::Field),
	qualifiers{false}
{
}

std::ostream& refureku::operator<<(std::ostream& out_stream, FieldInfo const& fieldInfo) noexcept
{
	out_stream << fieldInfo.type << "   " << fieldInfo.name << "   " << fieldInfo.properties;

	return out_stream;
}