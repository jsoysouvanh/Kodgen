#pragma once

#include <vector>
#include <clang-c/Index.h>

#include "Misc/FundamentalTypes.h"
#include "Misc/EAccessSpecifier.h"
#include "Parsing/ParsingSettings.h"
#include "Parsing/ParsingResults/FileParsingResult.h"
#include "Parsing/PropertyParser.h"
#include "InfoStructures/TypeInfo.h"
#include "InfoStructures/NamespaceInfo.h"

namespace kodgen
{
	class ParsingInfo
	{
		public:
			/**
			*	Settings used to parse headers
			*/
			ParsingSettings					parsingSettings;

			/**
			*	PropertyParser.
			*	used by the Parser to retrieve information
			*/
			PropertyParser					propertyParser;

			/**
			*	Current access modifier
			*/
			EAccessSpecifier				accessSpecifier	= EAccessSpecifier::Private;

			/**
			*	Current parsing class (if valid)
			*/
			opt::optional<StructClassInfo>	currentStructOrClass;

			/**
			*	Current parsing enum (if valid)
			*/
			opt::optional<EnumInfo>			currentEnum;

			/**
			*	Stack of namespaces
			*/
			std::vector<NamespaceInfo>		currentNamespaces;

			/**
			*	Final collected data
			*/
			FileParsingResult				parsingResult;

			ParsingInfo()	= default;
			~ParsingInfo()	= default;

			/**
			*	@return true if at least one error has occured during the parsing stage, else false
			*/
			bool	hasErrorOccured()									const	noexcept;

			/**
			*	@brief Tries to flush the current class or struct into the ParsingResult
			*
			*	@return true if a class/struct was flushed (currentStructOrClass had a value), else false
			*/
			bool	flushCurrentStructOrClass()									noexcept;

			/**
			*	@brief Tries to flush the current enum into the ParsingResult
			*
			*	@return true if an enum was flushed (currentEnum had a value), else false
			*/
			bool	flushCurrentEnum()											noexcept;

			/**
			*	@brief Tries to flush the current namespace into the ParsingResult
			*
			*	@return true if a namespace was flushed, else false
			*/
			bool	flushCurrentNamespace()										noexcept;

			/**
			*	Reset this parsingInfo fields
			*/
			void	reset()														noexcept;
	};
}