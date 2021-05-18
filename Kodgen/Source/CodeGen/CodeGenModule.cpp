#include <Kodgen/CodeGen/CodeGenModule.h>

#include <algorithm>

using namespace kodgen;

void CodeGenModule::registerPropertyRule(PropertyRule const& propertyRule) noexcept
{
	_propertyRules.push_back(&propertyRule);
}

bool CodeGenModule::unregisterPropertyRule(PropertyRule const& propertyRule) noexcept
{
	auto it = std::find(_propertyRules.cbegin(), _propertyRules.cend(), &propertyRule);

	if (it != _propertyRules.cend())
	{
		_propertyRules.erase(it);

		return true;
	}

	return false;
}

std::vector<PropertyRule const*> const& CodeGenModule::getPropertyRules() const noexcept
{
	return _propertyRules;
}