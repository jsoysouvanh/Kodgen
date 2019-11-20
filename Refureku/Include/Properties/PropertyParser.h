#pragma once

#include <set>

#include "Properties/PropertyRules.h"

namespace refureku
{
	class PropertyParser
	{
		public:
			char			propertySeparator			= ',';
			char			argumentSeparator			= ',';
			char			argumentGroupEnclosers[2]	= { '(', ')' };
			std::set<char>	ignoredCharacters;

			PropertyRules	classPropertyRules;
			PropertyRules	structPropertyRules;
			PropertyRules	fieldPropertyRules;
			PropertyRules	methodPropertyRules;
			PropertyRules	enumPropertyRules;

			//TODO
	};
}