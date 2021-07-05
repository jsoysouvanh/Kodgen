/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/CodeGenModule.h"

namespace kodgen
{
	//Forward declaration
	class MacroCodeGenEnv;

	/**
	*	Code generation module used with MacroCodeGenEnv environments.
	*/
	class MacroCodeGenModule : public CodeGenModule
	{
		protected:
			/**
			*	@brief Generate code in the header file header for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateHeaderFileHeaderCode(EntityInfo const*	entity,
																	 MacroCodeGenEnv&	env,
																	 std::string&		inout_result)	const	noexcept;

			/**
			*	@brief	Generate code in the class footer for the given entity.
			*			This method can only be called with a struct, class, field or method entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateClassFooterCode(EntityInfo const*	entity,
																MacroCodeGenEnv&	env,
																std::string&		inout_result)		const	noexcept;

			/**
			*	@brief Generate code in the header file footer for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateHeaderFileFooterCode(EntityInfo const*	entity,
																	 MacroCodeGenEnv&	env,
																	 std::string&		inout_result)	const	noexcept;

			/**
			*	@brief Generate code in the source file header for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateSourceFileHeaderCode(EntityInfo const*	entity,
																	 MacroCodeGenEnv&	env,
																	 std::string&		inout_result)	const	noexcept;

			/**
			*	@brief	Called just before calling generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode on a given entity.
			*			Can be used to initialize the environment or perform any pre-generation initialization.
			* 
			*	@param entity	Entity to generate code for.
			*	@param env		Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse.
			*			This method is the only one returning something different than ETraversalBehaviour::Break to make sure that
			*			any module inheriting from MacroCodeGenModule would iterate over all entities by default.
			*			Overriding this method and modifying the returned value will change the default iteration behaviour of the module.
			*/
			virtual ETraversalBehaviour	preGenerateCode(EntityInfo const*	entity,
														CodeGenEnv&			env)						const	noexcept;

			/**
			*	@brief	Called right after generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode have been called on a given entity.
			*			Can be used to modify the environment or perform any post-generation cleanup.
			* 
			*	@param entity	Entity to generate code for.
			*	@param env		Generation environment structure.
			* 
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	postGenerateCode(EntityInfo const*	entity,
														 CodeGenEnv&		env)						const	noexcept;

		public:
			virtual ~MacroCodeGenModule() = default;

			/**
			*	@brief	Make sure that the provided env is castable to MacroCodeGenEnv.
			*			The check can only happen if RTTI is enabled.
			* 
			*	@param env Generation environment structure.
			* 
			*	@return true if the 
			*/
			virtual bool				initialize(CodeGenEnv& env)						noexcept override;

			/**
			*	@brief Generate code using the provided environment as input.
			* 
			*	@param entity			Entity the module is generating code for. Might be nullptr, in which case the code is not generated for a specific entity.
			*	@param env				Environment provided by the FileGenerationUnit. You can cast environment to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param inout_result		String the method should append the generated code to.
			* 
			*	@return A combination of all the underlying calls returning a ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateCode(EntityInfo const*	entity,
													 CodeGenEnv&		env,
													 std::string&		inout_result)	noexcept override final;
	};
}