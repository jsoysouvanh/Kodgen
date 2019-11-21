#pragma once

#include "FundamentalTypes.h"

namespace refureku
{
	enum class EParsingError : uint16
	{
		/**
		*	The wrong macro has been used to mark an entity,
		*
		*	i.e. RfrkEnum(Prop1, Prop2) class MyClass {};
		*	Should use RfrkClass instead of RfrkEnum
		*/
		WrongPropertyMacroUsed = 0,

		/**
		*	A subproperty end encloser is missing
		*
		*	i.e. RfrkEnum(Prop1, Prop2[SubProp1, SubProp2, Prop3)
		*	The end encloser mark is missing after SubProp2
		*/
		SubPropertyEndEncloserMissing,

		/**
		*	An unregistered simple property has been detected in a Property declaration
		*	There might be a type miss, or you should register this simple property in the property rules
		*	of the marked entity type
		*/
		UnkownSimpleProperty,

		/**
		*	An unregistered complex property has been detected in a Property declaration
		*	There might be a type miss, or you should register this complex property in the property rules
		*	of the marked entity type
		*/
		UnkownComplexProperty,

		/**
		*	The file the parser attempted to parse doesn't exist or is not a file (might be a directory)
		*/
		InexistantFile,
		
		/**
		*	Might be used in the future.
		*	Is also used as a no-error value
		*/
		Count
	};
}