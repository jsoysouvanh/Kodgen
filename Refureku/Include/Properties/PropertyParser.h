#pragma once

#include <unordered_set>
#include <optional>

#include "Properties/PropertyRules.h"
#include "Properties/PropertyGroup.h"
#include "Properties/PropertyParsingError.h"

namespace refureku
{
	class PropertyParser
	{
		private:
			PropertyParsingError					_parsingError = PropertyParsingError::Count;

			std::vector<std::vector<std::string>>	_splitProps;

			bool									_hasCommonSeparator = true;

			/**
			*	Returns true on a successful split, else false
			*	On success, _splitProps is updated
			*	On failure, _parsingError is updated
			*/
			bool splitProperties(std::string&& propertiesString) noexcept;

			/**
			*	Return true on a successful split, else false
			*	On success, _splitProps is updated
			*	On failure, _parsingError is updated
			*/
			bool splitSubProperties(std::vector<std::vector<std::string>>& splitProps) noexcept;

			/**
			*	Removes all ignored characters from the string
			*/
			void cleanString(std::string& toCleanString) const noexcept;

		public:
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

			/**
			*	Called by the parser to internally setup some acceleration data
			*/
			void setup() noexcept;

			/**
			*	All get[...]Properties(std::string&& annotateMessage) below methods return an initialized optional
			*	if the annotate message is valid, else an uninitialized one.
			*/
			std::optional<PropertyGroup> getClassProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup> getStructProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup> getFieldProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup> getMethodProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup> getEnumProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup> getEnumValueProperties(std::string&& annotateMessage) noexcept;

			PropertyParsingError getParsingError() const noexcept;
	};
}