#pragma once

#include "FundamentalTypes.h"

namespace refureku
{
	enum class AccessSpecifier : uint8
	{
		Invalid		= 1 << 0,
		Public		= 1 << 1,
		Protected	= 1 << 2,
		Private		= 1 << 3
	};
}