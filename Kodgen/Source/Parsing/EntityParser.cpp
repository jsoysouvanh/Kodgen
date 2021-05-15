#include "Kodgen/Parsing/EntityParser.h"

#include <algorithm>

#include "Kodgen/Properties/NativeProperties/NativeProperties.h"

using namespace kodgen;

void EntityParser::updateShouldParseAllNested(EntityInfo const& parsingEntity) noexcept
{
	getContext().shouldParseAllNested = std::find_if(parsingEntity.propertyGroup.properties.cbegin(), parsingEntity.propertyGroup.properties.cend(),
													 [](Property const& prop) { return prop.name == NativeProperties::parseAllNestedProperty; })
												!= parsingEntity.propertyGroup.properties.cend();
}