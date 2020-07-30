#include "Properties/DefaultSimplePropertyRule.h"

using namespace kodgen;

DefaultSimplePropertyRule::DefaultSimplePropertyRule(std::string mainPropName, std::unordered_set<EntityInfo::EType> validEntityTypes) noexcept:
	mainPropName{std::move(mainPropName)},
	validEntityTypes{std::move(validEntityTypes)}
{
}

bool DefaultSimplePropertyRule::isMainPropSyntaxValid(std::string const& mainProperty, EntityInfo::EType entityType) const noexcept
{
	return mainProperty == mainPropName && validEntityTypes.find(entityType) != validEntityTypes.cend();
}