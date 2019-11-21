#pragma once

#include <unordered_set>

#include "Properties/PropertyRules.h"

namespace refureku
{
	struct PropertyParsingSettings
	{
		char						propertySeparator		= ',';
		char						subPropertySeparator	= ',';
		char						subPropertyEnclosers[2]	= { '[', ']' };
		std::unordered_set<char>	ignoredCharacters;

		PropertyRules				classPropertyRules;
		PropertyRules				structPropertyRules;
		PropertyRules				fieldPropertyRules;
		PropertyRules				methodPropertyRules;
		PropertyRules				enumPropertyRules;
		PropertyRules				enumValuePropertyRules;
	};
}