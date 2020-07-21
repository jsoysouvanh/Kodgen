#include "InfoStructures/NestedStructClassInfo.h"

using namespace kodgen;

NestedStructClassInfo::NestedStructClassInfo(StructClassInfo&& base, EAccessSpecifier accessSpecifier)	noexcept:
	accessSpecifier{accessSpecifier}
{
	parents			= std::move(base.parents);
	fields			= std::move(base.fields);
	methods			= std::move(base.methods);
	nestedClasses	= std::move(base.nestedClasses);
	nestedStructs	= std::move(base.nestedStructs);
}