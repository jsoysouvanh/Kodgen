#include "Kodgen/InfoStructures/VariableInfo.h"

#include <cassert>

using namespace kodgen;

VariableInfo::VariableInfo(CXCursor const& cursor, PropertyGroup&& propertyGroup, EEntityType entityType) noexcept:
	EntityInfo(cursor, std::forward<PropertyGroup>(propertyGroup), entityType),
	isStatic{false},	//TODO
	type(clang_getCursorType(cursor))
{
}

VariableInfo::VariableInfo(CXCursor const& cursor, PropertyGroup&& propertyGroup) noexcept:
	VariableInfo(cursor, std::forward<PropertyGroup>(propertyGroup), EEntityType::Variable)
{
}