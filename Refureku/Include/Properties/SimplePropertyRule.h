#pragma once

#include <string>

namespace refureku
{
	class SimplePropertyRule
	{
		public:
			std::string _name = "";

			SimplePropertyRule()							= default;
			SimplePropertyRule(std::string&& name)			noexcept;
			SimplePropertyRule(SimplePropertyRule const&)	= default;
			SimplePropertyRule(SimplePropertyRule&&)		= default;
			~SimplePropertyRule()							= default;

			bool hasSameName(SimplePropertyRule const& other)	const noexcept;

			bool operator<(SimplePropertyRule const& other)		const;
	};
}