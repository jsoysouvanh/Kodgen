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
	struct MacroCodeGenEnv;

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
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateHeaderFileHeaderCode(EntityInfo const*	entity,
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
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateClassFooterCode(EntityInfo const*	entity,
													MacroCodeGenEnv&	env,
													std::string&		inout_result)		const	noexcept;

			/**
			*	@brief Generate code in the header file footer for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateHeaderFileFooterCode(EntityInfo const*	entity,
														 MacroCodeGenEnv&	env,
														 std::string&		inout_result)	const	noexcept;

			/**
			*	@brief Generate code in the source file header for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateSourceFileHeaderCode(EntityInfo const*	entity,
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
			*	@return true if the pre-generation completed successfully, else false.
			*/
			virtual bool	preGenerateCode(EntityInfo const*	entity,
											CodeGenEnv&			env)						const	noexcept;

			/**
			*	@brief	Called right after generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode have been called on a given entity.
			*			Can be used to modify the environment or perform any post-generation cleanup.
			* 
			*	@param entity	Entity to generate code for.
			*	@param env		Generation environment structure.
			* 
			*	@return true if the post-generation completed successfully, else false.
			*/
			virtual bool	postGenerateCode(EntityInfo const*	entity,
											 CodeGenEnv&		env)						const	noexcept;

		public:
			/**
			*	@brief Generate code using the provided environment as input.
			* 
			*	@param entity			Entity the module is generating code for. Might be nullptr, in which case the code is not generated for a specific entity.
			*	@param env				Environment provided by the FileGenerationUnit. You can cast environment to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param inout_result		String the method should append the generated code to.
			* 
			*	@return true if the code generation completed successfully, else false. If false is returned
			*/
			virtual bool	generateCode(EntityInfo const*	entity,
										 CodeGenEnv&		env,
										 std::string&		inout_result)	const noexcept override;
	};
}