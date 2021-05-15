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
	class PropertyRule;

	struct Property
	{
		public:
			/** Name of this property. */
			std::string					name;

			/** Collection of all arguments of this property. */
			std::vector<std::string>	arguments;
	};
}