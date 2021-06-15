/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>
#include <vector>

#include "Kodgen/Misc/FundamentalTypes.h"
#include "Kodgen/CodeGen/ETraversalBehaviour.h"

namespace kodgen
{
	//Forward declaration
	class	PropertyCodeGen;
	class	CodeGenEnv;
	class	EntityInfo;

	//TODO: Add module dependency functionality?
	class CodeGenModule
	{
		private:
			/** Collection of all property code generators attached to this module. */
			std::vector<PropertyCodeGen const*> _propertyCodeGenerators;

			/**
			*	@brief Make eligible property code generators generate code for the provided entity.
			* 
			*	@param entity		Entity the module is generating code for. Might be nullptr, in which case the code is not generated for a specific entity.
			*	@param env			Data provided by the FileGenerationUnit. You can cast env to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param inout_result	String the method should append the generated code to.
			* 
			*	@return true if the code generation completed successfully for all property code generators, else false.
			*/
			bool	runPropertyCodeGenerators(EntityInfo const&		entity,
											  CodeGenEnv&			env,
											  std::string&			inout_result)	const	noexcept;

		public:
			virtual ~CodeGenModule() = default;

			/**
			*	@brief	Initialize the module and the provided environment. The module must make sure that the provided
			*			environment is valid to generate code with (i.e. that the environment inherits from all necessary class(es)
			*			for this module to work properly).
			*			The method is called by CodeGenUnit::preGenerateCode before any call to CodeGenModule::generateCode.
			*
			*	@param env Generation environment.
			* 
			*	@return true if the environment is valid and initialization completed successfully, else false.
			*/
			virtual bool								initialize(CodeGenEnv& env)								const	noexcept;

			/**
			*	@brief	Generate code using the provided environment as input.
			*			/!\ This method calls the code generation on registered and eligible property code generators, so this base
			*			implementation should be called in overrides. /!\
			* 
			*	@param entity			Entity the module is generating code for. Might be nullptr, in which case the code is not generated for a specific entity.
			*	@param env				Data provided by the FileGenerationUnit. You can cast env to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param inout_result		String the method should append the generated code to.
			* 
			*	@return true if the code generation completed successfully, else false. If false is returned
			*/
			virtual ETraversalBehaviour					generateCode(EntityInfo const*	entity,
																	 CodeGenEnv&		env,
																	 std::string&		inout_result)			const	noexcept;

			/**
			*	@brief	The generation order is a number defining in which order this module will generate code compared to other modules.
			*			Modules with a low generation order will execute first, and modules a high generation order will execute last.
			*			Modules having the same generation order value will execute in an undefined order.
			*/
			virtual int32								getGenerationOrder()									const	noexcept;

			/**
			*	@brief Add property rule to this generation module.
			* 
			*	@param propertyRule PropertyRule to register.
			*/
			void										addPropertyRule(PropertyCodeGen const& propertyRule)			noexcept;

			/**
			*	@brief Remove a property rule from this generation module.
			* 
			*	@param propertyRule PropertyRule to unregister.
			*/
			bool										removePropertyRule(PropertyCodeGen const& propertyRule)			noexcept;

			/**
			*	@brief Getter for _propertyRules field.
			*
			*	@return _propertyRules.
			*/
			std::vector<PropertyCodeGen const*> const&	getPropertyRules()										const	noexcept;
	};
}