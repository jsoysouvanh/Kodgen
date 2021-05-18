/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>
#include <vector>

#include "Kodgen/CodeGen/CodeGenData.h"
#include "Kodgen/Properties/PropertyRule.h"

namespace kodgen
{
	//TODO: Add module dependency functionality?
	class CodeGenModule
	{
		private:
			/** Collection of all property rules attached to this module. */
			std::vector<PropertyRule const*> _propertyRules;

		public:
			/**
			*	Number defining in which order this module will generate code compared to other modules.
			*	Modules with a lower generationOrder will execute first, and modules a high generationOrder will execute last.
			*	Modules having the same generationOrder value will execute in an undefined order.
			*/
			static constexpr int32 const generationOrder = 0;

			/**
			*	@brief Generate code for an entity using the provided data as input.
			* 
			*	@param entity			Data of the entity the module is generating code for.
			*	@param data				Data provided by the FileGenerationUnit. You can cast data to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param inout_result		String the method should append the generated code to.
			*	@param out_errorMessage	Error message to fill if an error happened during code generation.
			* 
			*	@return true if the code generation completed successfully, else false.
			*			If false is returned, out_errorMessage must be filled with an informative message describing why code generation failed.
			*/
			//TODO: Handle property code generation here
			virtual bool generateCode(EntityInfo const&		entity,
									  CodeGenData&	data,
									  std::string&			inout_result,
									  std::string&			out_errorMessage) const noexcept = 0;

			/**
			*	@brief Register a property rule to this generation module.
			* 
			*	@param propertyRule PropertyRule to register.
			*/
			void registerPropertyRule(PropertyRule const& propertyRule) noexcept;

			/**
			*	@brief Unregister a property rule from this generation module.
			* 
			*	@param propertyRule PropertyRule to unregister.
			*/
			bool unregisterPropertyRule(PropertyRule const& propertyRule) noexcept;

			/**
			*	@brief Getter for _propertyRules field.
			*
			*	@return _propertyRules.
			*/
			std::vector<PropertyRule const*> const& getPropertyRules() const noexcept;
	};
}