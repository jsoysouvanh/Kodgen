#include "Kodgen/Properties/PropertyRule.h"

using namespace kodgen;

bool PropertyRule::isValid(EntityInfo const& /* entity */, uint8 /* propertyIndex */) const noexcept
{
	//All properties are valid by default
	//Child classes should override this method to add validity constraints
	return true;
}

std::string	PropertyRule::generateCode(EntityInfo const& /* entity */, Property const& /* property */, void* /* userData */) const noexcept
{
	//Default implementation does not generate any code
	return "";
}