/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Kodgen/Config.h"
#include "Kodgen/Misc/FundamentalTypes.h"
#include "Kodgen/Misc/ICloneable.h"
#include "Kodgen/CodeGen/CodeGenEnv.h"
#include "Kodgen/InfoStructures/EntityInfo.h"

namespace kodgen
{
	class PropertyCodeGen : public ICloneable
	{
		protected:
			/**
			*	@brief Check if 2 entity types masks have at least one common entity type.
			* 
			*	@param lhs First entity type mask.
			*	@param rhs Second entity type mask.
			* 
			*	@return true if at least one entity type is common to the 2 provided masks, else false.
			*/
			static inline bool entityTypeOverlap(EEntityType lhs, EEntityType rhs)	noexcept;

		public:
			virtual ~PropertyCodeGen() = default;

			/**
			*	@brief Generate code for a given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateCode(EntityInfo const&	entity,
										 Property const&	property,
										 uint8				propertyIndex,
										 CodeGenEnv&		env,
										 std::string&		inout_result)					noexcept = 0;

			/**
			*	@brief Check if this property should generate code for the provided entity/property pair.
			*
			*	@param entity			Checked entity.
			*	@param property			Checked property.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*
			*	@return true if this property should generate code for the provided entity, else false.
			*/
			virtual bool	shouldGenerateCode(EntityInfo const&	entity,
											   Property const&		property,
											   uint8				propertyIndex)	const	noexcept = 0;

			/**
			*	@brief	Initialize the property code gen with the provided environment.
			*			The method is called by CodeGenModule::initialize before any call to PropertyCodeGen::generateCode.
			*			This default implementation does nothing and returns true by default.
			*
			*	@param env Generation environment.
			* 
			*	@return true if the initialization completed successfully, else false.
			*/
			virtual bool	initialize(CodeGenEnv& env)										noexcept;
	};

	#include "Kodgen/CodeGen/PropertyCodeGen.inl"
}