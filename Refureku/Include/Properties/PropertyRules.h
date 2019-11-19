#pragma once

#include <set>

#include "Properties/SimplePropertyRule.h"
#include "Properties/ComplexPropertyRule.h"

namespace refureku
{
	class PropertyRules
	{
		private:
			std::set<SimplePropertyRule>	_simplePropertyRules;
			std::set<ComplexPropertyRule>	_complexPropertyRules;

		public:
			PropertyRules()						= default;
			PropertyRules(PropertyRules const&)	= default;
			PropertyRules(PropertyRules&&)		= default;
			~PropertyRules()					= default;

			bool addSimpleProperty(SimplePropertyRule&& simplePropertyRule)			noexcept;
			bool removeSimpleProperty(SimplePropertyRule&& simplePropertyRule)		noexcept;
			bool addComplexProperty(ComplexPropertyRule&& complexPropertyRule)		noexcept;
			bool removeComplexProperty(ComplexPropertyRule&& complexPropertyRule)	noexcept;
	};
}