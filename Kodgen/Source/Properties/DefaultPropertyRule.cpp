#include "Kodgen/Properties/DefaultPropertyRule.h"

using namespace kodgen;

DefaultPropertyRule::DefaultPropertyRule(std::string const& propertyName, EEntityType validEntityTypes) noexcept:
	propertyName{propertyName},
	validEntityTypes{validEntityTypes}
{
}

bool DefaultPropertyRule::isValid(EntityInfo const& entity, uint8 propertyIndex) const noexcept
{
	return entity.propertyGroup.properties[propertyIndex].name == propertyName && (entity.entityType & validEntityTypes) != EEntityType::Undefined;
}