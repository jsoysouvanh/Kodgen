#pragma once

#include <vector>
#include <clang-c/Index.h>

#include "FundamentalTypes.h"
#include "AccessSpecifier.h"

#include "ClassInfo.h"

namespace refureku
{
	class ParsingInfo
	{
		public:
			/*	Level of class/struct parsing
			 *	0 if not parsing a class/struct,
			 *	1 if parsing a class/struct,
			 *	2 is parsing a nested class/struct
			 *	etc...
			**/
			uint8		classStructLevel				= 0;

			/*
			*	Clang cursor to the currently parsing class
			**/
			CXCursor	currentClassCursor				= clang_getNullCursor();

			/*
			 *	Is currently parsing an enum
			**/
			bool		isParsingEnum					= false;

			/*
			*	Is currently parsing a field
			**/
			bool		isParsingField					= false;

			/*
			*	Is currently parsing a method
			**/
			bool		isParsingMethod					= false;

			/*
			*	Clang cursor to the currently parsing entity (either an enum, a field or a method)
			**/
			CXCursor	currentEnumFieldMethodCursor	= clang_getNullCursor();

			/*
			*	Should check for any annotation
			**/
			bool		shouldCheckValidity			= false;

			/*
			 *	Current class modifier
			**/
			AccessSpecifier			accessSpecifier	= AccessSpecifier::Private;

			/*
			*	All collected class data
			**/
			std::vector<ClassInfo>	classInfos;

			/*
			*	//TODO struct data
			**/

			/*
			*	//TODO enumInfos;
			**/

			ParsingInfo()	= default;
			~ParsingInfo()	= default;

			void startStructParsing(CXCursor const& structCursor);
			void startClassParsing(CXCursor const& classCursor);
			void startFieldParsing(CXCursor const& fieldCursor);
			void startMethodParsing(CXCursor const& methodCursor);
			void startEnumParsing(CXCursor const& enumCursor);

			void endStructOrClassParsing();
			void endFieldParsing();
			void endMethodParsing();
			void endEnumParsing();

			void updateAccessSpecifier(CXCursor const& enumCursor);
	};
}