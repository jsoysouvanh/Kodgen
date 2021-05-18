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
	class CodeGenModuleGroup;

	class MacroCodeGenUnit : public CodeGenUnit
	{
		private:
			/**
			*	@brief Handle the whole code generation for a given entity with the provided data.
			* 
			*	@param entity	Entity we generate the code for.
			*	@param data		Generation data (really??)
			* 
			*	@return EIterationResult::Recurse if the traversal completed successfully.
			*			EIterationResult::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			EIterationResult::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			static EIterationResult generateEntityCode(EntityInfo const& entity, CodeGenData& data)	noexcept;

		protected:
			/**
			*	@brief	Generate code based on the provided parsing result.
			*			It is up to this method to create files to write to or not.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			virtual bool	generateCodeInternal(FileParsingResult const&	parsingResult,
												 FileGenerationResult&		out_genResult)	noexcept override;

		public:
			/** Pointer to a collection of all generation modules used by this generation unit. */
			CodeGenModuleGroup* codeGenModuleGroup;

			/**
			*	@brief Check whether the generated code for a given source file is up-to-date or not.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool	isUpToDate(fs::path const& sourceFile)					const noexcept override;
	};
}