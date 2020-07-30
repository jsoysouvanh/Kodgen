/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <vector>
#include <unordered_set>

#include "Properties/PropertyRules.h"
#include "Properties/SimplePropertyRule2.h"
#include "Properties/ComplexPropertyRule2.h"
#include "Misc/TomlUtility.h"

namespace kodgen
{
	class PropertyParsingSettings
	{
		public:
			/** Char used to separate two properties. */
			char								propertySeparator		= ',';

			/** Char used to separate two sub properties. */
			char								subPropertySeparator	= ',';

			/** Chars used to start and close respectively a group of sub properties. */
			char								subPropertyEnclosers[2]	= { '[', ']' };

			/** Collection of chars which will be ignored by the property parser. */
			std::unordered_set<char>			ignoredCharacters;

			/** Collection of all simple property rules. */
			std::vector<SimplePropertyRule2*>	simplePropertyRules;

			/** Collection of all complex property rules. */
			std::vector<ComplexPropertyRule2*>	complexPropertyRules;

			/** Macro to use to attach properties to a namespace. */
			std::string							namespaceMacroName		= "Namespace";

			/** Macro to use to attach properties to a class. */
			std::string							classMacroName			= "Class";

			/** Macro to use to attach properties to a struct. */
			std::string							structMacroName			= "Struct";

			/** Macro to use to attach properties to a field. */
			std::string							fieldMacroName			= "Field";

			/** Macro to use to attach properties to a method. */
			std::string							methodMacroName			= "Method";

			/** Macro to use to attach properties to an enum. */
			std::string							enumMacroName			= "Enum";

			/** Macro to use to attach properties to an enum value. */
			std::string							enumValueMacroName		= "EnumVal";

			PropertyParsingSettings()								= default;
			PropertyParsingSettings(PropertyParsingSettings const&)	= default;
			PropertyParsingSettings(PropertyParsingSettings&&)		= default;
			~PropertyParsingSettings()								= default;

			/**
			*	@brief Load settings from a TOML file.
			*	
			*	@param table TOML file table to look into.
			*/
			void	loadSettings(toml::value const& table)	noexcept;
	};
}