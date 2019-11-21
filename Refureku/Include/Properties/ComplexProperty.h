#pragma once

#include <string>
#include <vector>

#include "Properties/SimpleProperty.h"

namespace refureku
{
	class ComplexProperty final : public SimpleProperty
	{
		public:
			std::vector<std::string>	subProperties;

			ComplexProperty()						= default;
			ComplexProperty(ComplexProperty const&)	= default;
			ComplexProperty(ComplexProperty&&)		= default;
			~ComplexProperty()						= default;
	};
}