/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "InfoStructures/EntityInfo.h"
#include "Properties/PropertyGroup2.h"
#include "Misc/FundamentalTypes.h"

namespace kodgen
{
	class IPropertyRule
	{
		public:
			/**
			*	@brief	Check that a main property has a valid syntax for a given type of entity.
			*			This check happens right after the main property has been parsed.
			*
			*	@param mainProperty The property we are checking the syntax of.
			*	@param entityType	Type of the attached entity.
			*	
			*	@return true if the property syntax is valid, else false.
			*/
			virtual bool	isMainPropSyntaxValid(std::string const& mainProperty,
												  EntityInfo::EType	 entityType)		const noexcept = 0;
			
			/**
			*	@brief	Check that a property is valid inside a given property group.
			*			You could check for invalid double properties, or properties order for example.
			*			This check happens after the property group has been parsed, but before parsing the attached entity.
			*	
			*	@param propertyGroup	PropertyGroup containing the checked property.
			*	@param propertyIndex	Index of the property in the relevant propertyGroup array.
			*
			*	@return true if the property group is valid, else false.
			*/
			virtual bool	isPropertyGroupValid(PropertyGroup2 const&	propertyGroup,
												 uint16					propertyIndex)	const noexcept = 0;

			/**
			*	@brief	Check that the attached entity of a given property is valid.
			*			You could check the type of a field, or the prototype of a method for example.
			*			This check happens after the attached entity has been fully parsed.
			*
			*	@param entity			The entity attached to the checked property.
			*	@param propertyIndex	Index of the property in the relevant propertyGroup array.
			*/
			virtual bool	isEntityValid(EntityInfo const& entity,
										  uint16			propertyIndex)				const noexcept = 0;
	};
}