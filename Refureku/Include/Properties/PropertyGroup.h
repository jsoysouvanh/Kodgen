#pragma once

#include <set>

#include "Properties/SimpleProperty.h"
#include "Properties/ComplexProperty.h"

namespace refureku
{
	struct PropertyGroup
	{
		std::set<SimpleProperty>	simpleProperties;
		std::set<ComplexProperty>	complexProperties;
	};
}