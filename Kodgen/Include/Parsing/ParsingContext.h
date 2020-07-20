#pragma once

#include <clang-c/Index.h>

#include "ParsingResults/ParsingResultBase.h"
#include "Misc/EAccessSpecifier.h"

namespace kodgen
{
	//Forward declarations
	class	PropertyParser;
	struct	ParsingSettings;

	struct ParsingContext
	{
		public:
			/** Root cursor of this parsing context */
			CXCursor				rootCursor					= clang_getNullCursor();
			
			/** Current access specifier */
			EAccessSpecifier		currentAccessSpecifier		= EAccessSpecifier::Invalid;

			/** Should the validity of this entity be checked at the next parsing step? */
			bool					shouldCheckEntityValidity	= true;

			/** Parser used to parse entity properties */
			PropertyParser*			propertyParser				= nullptr;

			/** ParsingSettings used */
			ParsingSettings const*	parsingSettings				= nullptr;

			/** Result of the parsing */
			ParsingResultBase*		parsingResult				= nullptr;
	};
}