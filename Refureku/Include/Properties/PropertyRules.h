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

			bool addSimplePropertyRule(std::string&& propertyName)										noexcept;
			bool removeSimplePropertyRule(std::string&& propertyName)									noexcept;
			bool addComplexPropertyRule(std::string&& mainPropertyName, std::string&& subPropPattern)	noexcept;
			bool removeComplexPropertyRule(std::string&& mainPropertyName, std::string&& subPropPattern)noexcept;

			void clearSimplePropertyRules()																noexcept;
			void clearComplexPropertyRules()															noexcept;
			void clearAllPropertyRules()																noexcept;

			SimplePropertyRule const*	getSimplePropertyRule(std::string const& propertyName)	const	noexcept;
			ComplexPropertyRule const*	getComplexPropertyRule(std::string const& propertyName)	const	noexcept;
	};
}