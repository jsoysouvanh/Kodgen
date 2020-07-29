/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

namespace kodgen
{
	//Forward declaration
	class SimplePropertyRule2;

	class SimpleProperty2
	{
		public:
			/** Name of this property. */
			std::string					mainProperty;

			/** Pointer to the rule which validated this property syntax. */
			SimplePropertyRule2 const*	boundPropertyRule	= nullptr;

			SimpleProperty2()							= default;
			SimpleProperty2(std::string mainProperty)	noexcept;
			SimpleProperty2(SimpleProperty2 const&)		= default;
			SimpleProperty2(SimpleProperty2&&)			= default;
			~SimpleProperty2()							= default;

	};
}