/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

namespace kodgen
{
	class ICloneable
	{
		public:
			/**
			*	@brief Create a dynamically instantiated copy of the class implementing this method.
			* 
			*	@return A pointer on the newly instantiated copy of the implementing class.
			*/
			virtual ICloneable* clone() const noexcept = 0;
	};
}