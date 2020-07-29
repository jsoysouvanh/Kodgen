/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <vector>

#include "Properties/SimpleProperty2.h"
#include "Properties/ComplexProperty2.h"

namespace kodgen
{
	class PropertyGroup2
	{
		public:
			/** Vector containing all simple properties of an entity. */
			std::vector<SimpleProperty2>	simpleProperties;

			/** Vector containing all complex properties of an entity. */
			std::vector<ComplexProperty2>	complexProperties;

			PropertyGroup()						= default;
			PropertyGroup(PropertyGroup const&)	= default;
			PropertyGroup(PropertyGroup&&)		= default;
			~PropertyGroup()					= default;
	};
}