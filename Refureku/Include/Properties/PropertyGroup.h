#pragma once

#include <set>
#include <iostream>

#include "Properties/SimpleProperty.h"
#include "Properties/ComplexProperty.h"

namespace refureku
{
	struct PropertyGroup
	{
		std::set<SimpleProperty>	simpleProperties;
		std::set<ComplexProperty>	complexProperties;
	};

	std::ostream& operator<<(std::ostream& out_stream, PropertyGroup const&) noexcept;
}