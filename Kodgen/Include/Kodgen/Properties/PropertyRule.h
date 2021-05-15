/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Kodgen/InfoStructures/EntityInfo.h"
#include "Kodgen/Properties/PropertyGroup.h"
#include "Kodgen/Misc/FundamentalTypes.h"

namespace kodgen
{
	class PropertyRule
	{
		public:
			PropertyRule()						= default;
			PropertyRule(PropertyRule const&)	= default;
			PropertyRule(PropertyRule&&)		= default;
			virtual ~PropertyRule()				= default;

			/**
			*	@brief	Check if this property rule is valid to be bound to the property of a given entity.
			*			This check happens after all properties & its attached entity has been fully parsed.
			*
			*	@param entity			The entity attached to the checked property.
			*	@param propertyIndex	Index of the property in the propertyGroup.
			*
			*	@return true if the entity is valid for this property rule, else false.
			*/
			virtual bool		isValid(EntityInfo const&	entity,
										uint8				propertyIndex)		const	noexcept;

			/**
			*	@brief Entry point to generate code for a given entity and property.
			*	
			*	@param entity	Entity the property is attached to.
			*	@param property	Property bound to this rule.
			*	@param userData	Any data the user might want to pass.
			*	
			*	@return Generated code as string.
			*/
			virtual std::string	generateCode(EntityInfo const&	entity,
											 Property const&	property,
											 void*				userData)		const	noexcept;
	};
}