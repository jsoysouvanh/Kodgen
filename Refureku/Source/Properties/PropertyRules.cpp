#include "Properties/PropertyRules.h"

#include <algorithm>

using namespace refureku;

bool PropertyRules::addSimpleProperty(SimplePropertyRule&& simplePropertyRule) noexcept
{
	return _simplePropertyRules.insert(std::forward<SimplePropertyRule>(simplePropertyRule)).second;
}

bool PropertyRules::removeSimpleProperty(SimplePropertyRule&& simplePropertyRule) noexcept
{
	std::set<SimplePropertyRule>::const_iterator it = _simplePropertyRules.find(simplePropertyRule);

	if (it != _simplePropertyRules.cend())
	{
		_simplePropertyRules.erase(it);
		
		return true;
	}

	return false;
}

bool PropertyRules::addComplexProperty(ComplexPropertyRule&& complexPropertyRule) noexcept
{
	return _complexPropertyRules.insert(std::forward<ComplexPropertyRule>(complexPropertyRule)).second;
}

bool PropertyRules::removeComplexProperty(ComplexPropertyRule&& complexPropertyRule) noexcept
{
	std::set<ComplexPropertyRule>::const_iterator it = _complexPropertyRules.find(complexPropertyRule);

	if (it != _complexPropertyRules.cend())
	{
		_complexPropertyRules.erase(it);

		return true;
	}

	return false;
}

void PropertyRules::clearSimpleProperties() noexcept
{
	_simplePropertyRules.clear();
}

void PropertyRules::clearComplexProperties() noexcept
{
	_complexPropertyRules.clear();
}

void PropertyRules::clear() noexcept
{
	clearSimpleProperties();
	clearComplexProperties();
}

SimplePropertyRule const* PropertyRules::getSimpleProperty(std::string const& propertyName) const noexcept
{
	decltype(_simplePropertyRules)::const_iterator it = std::find_if(_simplePropertyRules.cbegin(), _simplePropertyRules.cend(), [propertyName](SimplePropertyRule const& prop) { return prop.name == propertyName; });

	return (it != _simplePropertyRules.cend()) ? &*it : nullptr;
}

ComplexPropertyRule const* PropertyRules::getComplexProperty(std::string const& propertyName) const noexcept
{
	decltype(_complexPropertyRules)::const_iterator it = std::find_if(_complexPropertyRules.cbegin(), _complexPropertyRules.cend(), [propertyName](ComplexPropertyRule const& prop) { return prop.name == propertyName; });

	return (it != _complexPropertyRules.cend()) ? &*it : nullptr;
}