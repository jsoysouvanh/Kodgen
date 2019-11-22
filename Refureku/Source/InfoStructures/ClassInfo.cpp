#include "InfoStructures/ClassInfo.h"

using namespace refureku;

ClassInfo::ClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept:
	StructClassInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
}

std::ostream& refureku::operator<<(std::ostream& out_stream, ClassInfo const& classInfo) noexcept
{
	out_stream << "Class : " << classInfo.name << std::endl;
	out_stream << classInfo.properties;

	return out_stream;
}