#pragma once

#include "FundamentalTypes.h"

namespace refureku
{
	enum class PropertyParsingError : uint8
	{
		WrongPropertyMacroUsed = 0,
		SubPropertyEndEncloserMissing,
		UnkownSimpleProperty,
		UnkownComplexProperty,
		
		Count	//Also used as no-error
	};
}