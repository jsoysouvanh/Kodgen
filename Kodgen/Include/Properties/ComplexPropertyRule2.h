/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Properties/SimplePropertyRule2.h"

namespace kodgen
{
	class ComplexPropertyRule2 : public SimplePropertyRule2
	{
		public:
			ComplexPropertyRule2()								= default;
			ComplexPropertyRule2(ComplexPropertyRule2 const&)	= default;
			ComplexPropertyRule2(ComplexPropertyRule2&&)		= default;
			~ComplexPropertyRule2()								= default;

			/**
			*	@brief	Check that a sub property has a valid syntax for a given type of entity.
			*			This check happens right after the sub property has been parsed.
			*
			*	@param subProperty	The property we are checking the syntax of.
			*	@param entityType	Type of the attached entity.
			*	
			*	@return true if the property syntax is valid, else false.
			*/
			virtual bool	isSubPropSyntaxValid(std::string const&	subProperty,
												 EntityInfo::EType	entityType)		const noexcept = 0;
	};
}