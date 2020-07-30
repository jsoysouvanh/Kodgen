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
		protected:
			/**
			*	@brief Check that a property rule is used only once in a given property group.
			*	
			*	@param propertyGroup		The property group containing the checked property.
			*	@param propertyIndex		Index of the property that should appear only once.
			*	@param out_errorDescription	Error to fill in case the property rule is used more than once.
			*
			*	@return true if this property rule is used by a single property in the property group, else false.
			*			If false is returned, out_errorDescription must be filled with an error description.
			*/
			virtual bool	isUsedOnlyOnce(PropertyGroup2 const&	propertyGroup,
										   uint8					propertyIndex,
										   std::string&				out_errorDescription)	const noexcept;

		public:
			SimplePropertyRule2()							= default;
			SimplePropertyRule2(SimplePropertyRule2 const&)	= default;
			SimplePropertyRule2(SimplePropertyRule2&&)		= default;
			virtual ~SimplePropertyRule2()					= default;
	};
}