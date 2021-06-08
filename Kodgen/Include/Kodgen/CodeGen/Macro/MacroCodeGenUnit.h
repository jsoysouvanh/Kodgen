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
	class	CodeGenModuleGroup;
	class	MacroCodeGenUnitSettings;
	struct	MacroCodeGenEnv;

	class MacroCodeGenUnit : public CodeGenUnit
	{
		private:
			/**
			*	@brief Handle the whole code generation for a given entity with the provided environment.
			* 
			*	@param entity	Entity we generate the code for.
			*	@param env		Generation environment.
			* 
			*	@return EIterationResult::Recurse if the traversal completed successfully.
			*			EIterationResult::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			EIterationResult::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			EIterationResult		generateEntityCode(EntityInfo const&	entity,
													   CodeGenEnv&			env)					noexcept;

			/**
			*	@brief Handle the code generation for class footer code gen location.
			* 
			*	@param entity	Entity we generate the code for. Must be one of Struct/Class/Field/Method.
			*	@param env		Generation environment.
			* 
			*	@return true if the entity class footer code was generated successfully, else false.
			*/
			bool					generateEntityClassFooterCode(EntityInfo const&	entity,
																  MacroCodeGenEnv&	env)			noexcept;

		protected:
			/**
			*	@brief	Generate code based on the provided parsing result.
			*			Generated code will be dispatched in 2 different files (1 header, 1 source).
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param env				Generation environment.
			* 
			*	@return true if the code generation completed successfully without error, else false.
			*/
			virtual bool	generateCodeInternal(FileParsingResult const&	parsingResult,
												 CodeGenEnv&				env)				noexcept	override;

			/**
			*	@brief	Check that the env object is castable to MacroCodeGenEnv,
			*			and is also valid for each registered module, then setup the environment.
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool		preGenerateCode(FileParsingResult const&	parsingResult,
												CodeGenEnv&					env)										noexcept;

			/**
			*	@brief	(Re)generate the header file.
			* 
			*	@param parsingResult	Result of the parsing process.
			*	@param env				Generation environment.
			*/
			void			generateHeaderFile(FileParsingResult const& parsingResult,
											   MacroCodeGenEnv&			env)			const	noexcept;

			/**
			*	@brief	(Re)generate the source file.
			* 
			*	@param parsingResult	Result of the parsing process.
			*	@param env				Generation environment.
			*/
			void			generateSourceFile(FileParsingResult const& parsingResult,
											   MacroCodeGenEnv&			env)			const	noexcept;

			/**
			*	@brief Compute the path of the header file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the header file generated from the provided source file.
			*/
			fs::path		getGeneratedHeaderFilePath(fs::path const& sourceFile)		const	noexcept;

			/**
			*	@brief Compute the path of the source file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the source file generated from the provided source file.
			*/
			fs::path		getGeneratedSourceFilePath(fs::path const& sourceFile)		const	noexcept;

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
			*	@brief Setter for the inherited settings field with suitable derived settings class.
			*/
			void			setSettings(MacroCodeGenUnitSettings const* cguSettings)	noexcept;
	};
}