#include "Properties/ComplexPropertyRule.h"

using namespace refureku;

ComplexPropertyRule::ComplexPropertyRule(std::string&& name, std::string&& argumentPattern)	noexcept:
	SimplePropertyRule(std::forward<std::string>(name)),
	_argumentPattern{std::forward<std::string>(argumentPattern)}
{}

bool ComplexPropertyRule::isValidArgument(std::string const& argument) const noexcept
{
	std::match_results<std::string::const_iterator>	matchResults;

	if (std::regex_search(argument, matchResults, _argumentPattern, std::regex_constants::match_default))
	{
		return matchResults.prefix().str().empty() && matchResults.suffix().str().empty();
	}

	return false;
}

//bool ComplexPropertyRule::operator<(ComplexPropertyRule const& other) const
//{
//	return _name < other._name;
//}