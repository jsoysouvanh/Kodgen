#include "Properties/SimplePropertyRule.h"

using namespace refureku;

SimplePropertyRule::SimplePropertyRule(std::string&& name) noexcept:
	_name{std::forward<std::string>(name)}
{}

bool SimplePropertyRule::hasSameName(SimplePropertyRule const& other) const noexcept
{
	return _name == other._name;
}

bool SimplePropertyRule::operator<(SimplePropertyRule const& other) const
{
	return _name < other._name;
}