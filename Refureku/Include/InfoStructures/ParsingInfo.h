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
			ParsingSettings const*	parsingSettings = nullptr;

			/**
			*	PropertyParser.
			*	used by the Parser to retrieve information
			*/
			PropertyParser			propertyParser;

			/*
			*	Current class modifier
			**/
			EAccessSpecifier		accessSpecifier	= EAccessSpecifier::Private;

			/**
			*	Final collected data
			*/
			ParsingResult			parsingResult;

			ParsingInfo()	= default;
			~ParsingInfo()	= default;

			bool					hasErrorOccured()									const	noexcept;

			void					setParsingSettings(ParsingSettings const* parsingSettings)	noexcept;
	};
}