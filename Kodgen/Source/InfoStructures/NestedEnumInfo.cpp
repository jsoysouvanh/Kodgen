#include "Kodgen/InfoStructures/NestedEnumInfo.h"

using namespace kodgen;

NestedEnumInfo::NestedEnumInfo(EnumInfo&& base, EAccessSpecifier accessSpecifier) noexcept:
	accessSpecifier{accessSpecifier}
{
	entityType		= base.entityType;
	name			= std::move(base.name);
	id				= std::move(base.id);
	propertyGroup	= std::move(base.propertyGroup);

	type			= std::move(base.type);
	underlyingType	= std::move(base.underlyingType);
	enumValues		= std::move(base.enumValues);
}