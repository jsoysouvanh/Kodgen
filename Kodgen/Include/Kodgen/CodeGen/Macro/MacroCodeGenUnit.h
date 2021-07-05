/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/CodeGenUnit.h"

namespace kodgen
{
	//Forward declaration
	class	MacroCodeGenUnitSettings;
	class	MacroCodeGenEnv;

	class MacroCodeGenUnit final : public CodeGenUnit
	{
		private:
			/**
			*	@brief Handle the code generation for class footer code gen location.
			* 
			*	@param codeGenModule	Module calling the generateCode method.
			*	@param entity			Entity we generate the code for. Must be one of Struct/Class/Field/Method.
			*	@param env				Generation environment.
			* 
			*	@return An EIterationBehaviour instructing how the entity traversal should continue (see the EIterationResult documentation for more info).
			*			The result of the codeGenModule->generateCode() call can be forwarded in most cases to let the module control the flow of the traversal.
			*/
			ETraversalBehaviour	generateEntityClassFooterCode(CodeGenModule&	codeGenModule,
															  EntityInfo const&	entity,
															  MacroCodeGenEnv&	env)			noexcept;

		protected:
			/**
			*	@brief	Execute the codeGenModule->generateCode method 4 times with the given entity and environment, by
			*			updating the environment between each call (MacroCodeGenEnv::codeGenLocation and
			*			MacroCodeGenEnv::separator are updated).
			*
			*	@param codeGenModule	Module calling the generateCode method.
			*	@param entity			Target entity for this code generation pass.
			*	@param env				Generation environment structure.
			* 
			*	@return An EIterationBehaviour instructing how the entity traversal should continue (see the EIterationResult documentation for more info).
			*			The result of the codeGenModule->generateCode() call can be forwarded in most cases to let the module control the flow of the traversal.
			*/
			virtual ETraversalBehaviour	runCodeGenModuleOnEntity(CodeGenModule&		codeGenModule,
																 EntityInfo const&	entity,
																 CodeGenEnv&		env)					noexcept	override;

			/**
			*	@brief	Create/update the header and source files and fill them with the generated code.
			* 
			*	@param env				Generation environment structure.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool				postGenerateCode(CodeGenEnv& env)									noexcept	override;

			/**
			*	@brief	(Re)generate the header file.
			* 
			*	@param env Generation environment.
			*/
			void						generateHeaderFile(MacroCodeGenEnv&	env)					const	noexcept;

			/**
			*	@brief	(Re)generate the source file.
			* 
			*	@param env Generation environment.
			*/
			void						generateSourceFile(MacroCodeGenEnv&	env)					const	noexcept;

			/**
			*	@brief Compute the path of the header file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the header file generated from the provided source file.
			*/
			fs::path					getGeneratedHeaderFilePath(fs::path const& sourceFile)		const	noexcept;

			/**
			*	@brief Compute the path of the source file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the source file generated from the provided source file.
			*/
			fs::path					getGeneratedSourceFilePath(fs::path const& sourceFile)		const	noexcept;

		public:
			/**
			*	@brief	Check that both the generated header and source files are newer than the source file.
			*			If the generated header file doesn't exist, create it and leave it empty.
			*			We do that because since the generated header is included in the source code,
			*			it could generate an undefined behaviour if the header doesn't exist.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool	isUpToDate(fs::path const& sourceFile)				const	noexcept	override;

			/**
			*	@brief Create a MacroCodeGenEnv object and forward it to generateCodeInternal.
			*
			*	@param parsingResult Result of a file parsing used to generate the new file.
			* 
			*	@return false if the generation process was aborted prematurely because of any error, else true.
			*/
			virtual bool	generateCode(FileParsingResult const&	parsingResult)		noexcept	override;

			/**
			*	@brief Setter for the inherited settings field with suitable derived settings class.
			*/
			void			setSettings(MacroCodeGenUnitSettings const* cguSettings)	noexcept;
	};
}