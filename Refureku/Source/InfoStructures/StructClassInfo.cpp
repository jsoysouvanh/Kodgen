#include "InfoStructures/StructClassInfo.h"

using namespace refureku;

StructClassInfo::StructClassInfo() noexcept
{
	initContainers();
}

StructClassInfo::StructClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup, EType&& entityType) noexcept:
	EntityInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup), std::forward<EType>(entityType))
{
	initContainers();
}

void StructClassInfo::initContainers() noexcept
{
	fields.reserve(3u);
	fields.emplace(EAccessSpecifier::Public, std::vector<FieldInfo>());
	fields.emplace(EAccessSpecifier::Protected, std::vector<FieldInfo>());
	fields.emplace(EAccessSpecifier::Private, std::vector<FieldInfo>());

	methods.reserve(3u);
	methods.emplace(EAccessSpecifier::Public, std::vector<MethodInfo>());
	methods.emplace(EAccessSpecifier::Protected, std::vector<MethodInfo>());
	methods.emplace(EAccessSpecifier::Private, std::vector<MethodInfo>());
}