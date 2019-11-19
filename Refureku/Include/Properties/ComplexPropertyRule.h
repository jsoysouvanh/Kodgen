#pragma once

#include <regex>

#include "Properties/SimplePropertyRule.h"

namespace refureku
{
	class ComplexPropertyRule final : public SimplePropertyRule
	{
		public:
			std::regex	_argumentPattern;	//regex
			
			ComplexPropertyRule()													= default;
			ComplexPropertyRule(std::string&& name, std::string&& argumentPattern)	noexcept;
			ComplexPropertyRule(ComplexPropertyRule const&)							= default;
			ComplexPropertyRule(ComplexPropertyRule&&)								= default;
			~ComplexPropertyRule()													= default;

			bool isValidArgument(std::string const& argument)	const noexcept;

			//bool operator<(ComplexPropertyRule const& other)	const;
	};
}