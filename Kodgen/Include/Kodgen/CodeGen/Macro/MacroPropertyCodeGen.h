/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/PropertyCodeGen.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenEnv.h"

namespace kodgen
{
	class MacroPropertyCodeGen : public PropertyCodeGen
	{
		protected:
			/**
			*	@brief Generate code in the header file header for the given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateHeaderFileHeaderCode(EntityInfo const&	entity,
														 Property const&	property,
														 uint8				propertyIndex,
														 MacroCodeGenEnv&	env,
														 std::string&		inout_result)	const	noexcept;

			/**
			*	@brief	Generate code in the class footer for the given entity.
			*			This method can only be called with a struct, class, field or method entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateClassFooterCode(EntityInfo const&	entity,
													Property const&		property,
													uint8				propertyIndex,
													MacroCodeGenEnv&	env,
													std::string&		inout_result)		const	noexcept;

			/**
			*	@brief Generate code in the header file footer for the given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateHeaderFileFooterCode(EntityInfo const&	entity,
														 Property const&	property,
														 uint8				propertyIndex,
														 MacroCodeGenEnv&	env,
														 std::string&		inout_result)	const	noexcept;

			/**
			*	@brief Generate code in the source file header for the given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateSourceFileHeaderCode(EntityInfo const&	entity,
														 Property const&	property,
														 uint8				propertyIndex,
														 MacroCodeGenEnv&	env,
														 std::string&		inout_result)	const	noexcept;

			/**
			*	@brief	Called just before calling generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode on a given entity.
			*			Can be used to initialize the data structure or perform any pre generation initialization.
			* 
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the pre-generation completed successfully, else false.
			*/
			virtual bool	preGenerateCode(EntityInfo const&	entity,
											Property const&		property,
											uint8				propertyIndex,
											CodeGenEnv&			env)						const	noexcept;

			/**
			*	@brief	Called right after generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode have been called on a given entity.
			*			Can be used to modify the data structure or perform any post generation cleanup.
			* 
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the post-generation completed successfully, else false.
			*/
			virtual bool	postGenerateCode(EntityInfo const&	entity,
											 Property const&	property,
											 uint8				propertyIndex,
											 CodeGenEnv&		env)						const	noexcept;

		public:
			/**
			*	@brief Make sure that the environment is castable to MacroCodeGenEnv.
			* 
			*	@param env Generation environment structure.
			* 
			*	@return true if the environment is castable to MacroCodeGenEnv, else false.
			*/
			virtual bool	initialize(CodeGenEnv& env)						const noexcept override;

			/**
			*	@brief	Generate code for a given entity.
			*			This method analyzes the code location retrieved from the MacroCodeGenEnv
			*			and dispatch the call to the relevant method.
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
										 std::string&		inout_result)	const noexcept override;
	};
}