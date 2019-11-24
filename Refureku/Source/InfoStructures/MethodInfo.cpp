#include "InfoStructures/MethodInfo.h"

using namespace refureku;

MethodInfo::MethodInfo(std::string&& entityName, PropertyGroup&& propertyGroup) noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
}

std::ostream& refureku::operator<<(std::ostream& out_stream, MethodInfo const& methodInfo) noexcept
{
	out_stream << methodInfo.returnType << "   " << methodInfo.name << "   " << methodInfo.properties;

	return out_stream;
}