/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <vector>

#include "Kodgen/Properties/Property.h"

namespace kodgen
{
	struct PropertyGroup
	{
		/** Collection containing all properties of an entity. */
		std::vector<Property> properties;
	};
}