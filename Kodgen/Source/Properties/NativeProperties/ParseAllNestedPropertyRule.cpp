#include "Kodgen/Properties/NativeProperties/ParseAllNestedPropertyRule.h"

#include "Kodgen/Properties/NativeProperties/NativeProperties.h"

using namespace kodgen;

ParseAllNestedPropertyRule::ParseAllNestedPropertyRule() noexcept:
	DefaultPropertyRule(NativeProperties::parseAllNestedProperty,
						EEntityType::Namespace | EEntityType::Struct | EEntityType::Class)
{
}