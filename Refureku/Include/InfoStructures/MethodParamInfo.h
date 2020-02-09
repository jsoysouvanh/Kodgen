#pragma once

#include <string>

#include "Misc/FundamentalTypes.h"
#include "InfoStructures/TypeInfo.h"

namespace refureku
{
	struct MethodParamInfo
	{
		public:
			/**
			*	Parameter type
			*/
			TypeInfo	type;

			/**
			*	Parameter name. Can be empty if no name.
			*/
			std::string	name;
	};
}