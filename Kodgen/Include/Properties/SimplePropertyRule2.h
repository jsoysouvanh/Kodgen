/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Properties/IPropertyRule.h"

namespace kodgen
{
	class SimplePropertyRule2 : public IPropertyRule
	{
		public:
			SimplePropertyRule2()							= default;
			SimplePropertyRule2(SimplePropertyRule2 const&)	= default;
			SimplePropertyRule2(SimplePropertyRule2&&)		= default;
			~SimplePropertyRule2()							= default;
	};
}