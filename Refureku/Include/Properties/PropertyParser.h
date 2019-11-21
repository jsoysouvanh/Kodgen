#pragma once

#include <optional>

#include "Properties/PropertyGroup.h"
#include "Properties/EParsingError.h"
#include "Properties/PropertyParsingSettings.h"

namespace refureku
{
	class PropertyParser
	{
		private:
			PropertyParsingSettings const*			_propertyParsingSettings	= nullptr;
			bool									_hasCommonSeparator			= true;

			EParsingError							_parsingError				= EParsingError::Count;
			std::vector<std::vector<std::string>>	_splitProps;

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

			/**
			*	Check for each prop / subprop validity and fill a propertyGroup consequently
			*	On success, returns the filled propertyGroup object
			*	On failure, returns an empty empty optional object
			*/
			std::optional<PropertyGroup> checkAndFillClassPropertyGroup(std::vector<std::vector<std::string>>& splitProps) noexcept;

		public:
			/**
			*	Called by the parser to internally setup some acceleration data
			*/
			void setup(PropertyParsingSettings const* propertyParsingSettings) noexcept;

			/**
			*	Clear all collected data such as parsingErrors or split props.
			*	Called to have a clean state and prepare for a new parse
			*/
			void clean() noexcept;

			/**
			*	All get[...]Properties(std::string&& annotateMessage) below methods return an initialized optional
			*	if the annotate message is valid, else an uninitialized one.
			*/
			std::optional<PropertyGroup>	getClassProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup>	getStructProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup>	getFieldProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup>	getMethodProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup>	getEnumProperties(std::string&& annotateMessage) noexcept;
			std::optional<PropertyGroup>	getEnumValueProperties(std::string&& annotateMessage) noexcept;

			EParsingError					getParsingError() const noexcept;
	};
}