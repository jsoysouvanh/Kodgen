#include "Properties/PropertyRules.h"

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