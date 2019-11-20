#pragma once

#include <set>
#include <optional>

#include "Properties/PropertyRules.h"
#include "Properties/PropertyGroup.h"

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
			PropertyRules	enumValue0PropertyRules;

			std::optional<PropertyGroup> getClassProperties(std::string&& annotateMessage) const;
			std::optional<PropertyGroup> getStructProperties(std::string&& annotateMessage) const;
			std::optional<PropertyGroup> getFieldProperties(std::string&& annotateMessage) const;
			std::optional<PropertyGroup> getMethodProperties(std::string&& annotateMessage) const;
			std::optional<PropertyGroup> getEnumProperties(std::string&& annotateMessage) const;
	};
}