#include "Properties/SimplePropertyRule.h"

using namespace refureku;

SimplePropertyRule::SimplePropertyRule(std::string&& name) noexcept:
	name{std::forward<std::string>(name)}
{}

bool SimplePropertyRule::hasSameName(SimplePropertyRule const& other) const noexcept
{
	return name == other.name;
}

bool SimplePropertyRule::operator<(SimplePropertyRule const& other) const
{
	return name < other.name;
}