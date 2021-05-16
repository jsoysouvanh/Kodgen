/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/FileGenerationUnit.h"

namespace kodgen
{
	class MacroFileGenerationUnit : public FileGenerationUnit
	{
		protected:
			/**
			*	@brief	Generate code based on the provided parsing result.
			*			It is up to this method to create files to write to or not.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			virtual void	generateCodeInternal(FileParsingResult const&	parsingResult,
												 FileGenerationResult&		out_genResult)	noexcept override;

		public:
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