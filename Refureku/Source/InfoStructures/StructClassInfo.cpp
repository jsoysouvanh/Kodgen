#include "InfoStructures/StructClassInfo.h"

using namespace refureku;

StructClassInfo::StructClassInfo() noexcept
{
	initContainers();
}

StructClassInfo::StructClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup) noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
	initContainers();
}

void StructClassInfo::initContainers() noexcept
{
	fields.emplace(EAccessSpecifier::Public, std::vector<FieldInfo>());
	fields.emplace(EAccessSpecifier::Protected, std::vector<FieldInfo>());
	fields.emplace(EAccessSpecifier::Private, std::vector<FieldInfo>());

	methods.emplace(EAccessSpecifier::Public, std::vector<MethodInfo>());
	methods.emplace(EAccessSpecifier::Protected, std::vector<MethodInfo>());
	methods.emplace(EAccessSpecifier::Private, std::vector<MethodInfo>());

}