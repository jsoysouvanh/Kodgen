#pragma once

#include <vector>
#include <clang-c/Index.h>

#include "Misc/FundamentalTypes.h"
#include "Misc/EAccessSpecifier.h"
#include "Parsing/ParsingSettings.h"
#include "Parsing/ParsingResult.h"
#include "Properties/PropertyParser.h"
#include "InfoStructures/ClassInfo.h"
#include "InfoStructures/TypeInfo.h"

namespace refureku
{
	class ParsingInfo
	{
		public:
			/**
			*
			*/
			ParsingSettings const*		parsingSettings = nullptr;

			/**
			*	PropertyParser.
			*	used by the Parser to retrieve information
			*/
			PropertyParser				propertyParser;

			/**
			*	Current class modifier
			*/
			EAccessSpecifier			accessSpecifier	= EAccessSpecifier::Private;

			/**
			*	Current parsing class (if valid)
			*/
			std::optional<ClassInfo>	currentClass;

			/**
			*	Current parsing enum (if valid)
			*/
			std::optional<EnumInfo>		currentEnum;

			/**
			*	Final collected data
			*/
			ParsingResult				parsingResult;

			ParsingInfo()	= default;
			~ParsingInfo()	= default;

			/**
			*	@return true if at least one error has occured during the parsing stage, else false
			*/
			bool	hasErrorOccured()									const	noexcept;

			/**
			*	@brief Tries to flush the current class into the ParsingResult
			*
			*	@return true if a class was flushed (currentClass had a value), else false
			*/
			bool	flushCurrentClass()											noexcept;

			/**
			*	@brief Tries to flush the current enum into the ParsingResult
			*
			*	@return true if an enum was flushed (currentEnum had a value), else false
			*/
			bool	flushCurrentEnum()											noexcept;

			void	setParsingSettings(ParsingSettings const* parsingSettings)	noexcept;
	};
}