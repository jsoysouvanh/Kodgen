/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>
#include <vector>

namespace kodgen
{
	//Forward declaration
	class ComplexPropertyRule2;

	class ComplexProperty2
	{
		public:
			/** Name of this property. */
			std::string					mainProperty;

			/** Collection of all subproperties. */
			std::vector<std::string>	subProperties;

			/** Pointer to the rule which validated this property syntax. */
			ComplexPropertyRule2 const*	boundPropertyRule	= nullptr;

			ComplexProperty2()							= default;
			ComplexProperty2(std::string mainProperty)	noexcept;
			ComplexProperty2(ComplexProperty2 const&)	= default;
			ComplexProperty2(ComplexProperty2&&)		= default;
			~ComplexProperty2()							= default;
	};
}