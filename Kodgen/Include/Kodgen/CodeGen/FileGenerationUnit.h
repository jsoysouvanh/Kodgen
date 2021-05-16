/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/FileGenerationSettings.h"
#include "Kodgen/CodeGen/FileGenerationResult.h"
#include "Kodgen/Parsing/ParsingResults/FileParsingResult.h"
#include "Kodgen/Misc/ILogger.h"
#include "Kodgen/Misc/Filesystem.h"
#include "Kodgen/Misc/FundamentalTypes.h"

namespace kodgen
{
	class FileGenerationUnit
	{
		//FileGenerator can access FileGenerationUnit class to forward logger and settings
		friend class FileGenerator;

		protected:
			/** Logger used to issue logs from the FileGenerationUnit. */
			ILogger*						logger		= nullptr;

			/** Generation settings. */
			FileGenerationSettings const*	settings	= nullptr;

			/**
			*	@brief	Generate code based on the provided parsing result.
			*			It is up to this method to create files to write to or not.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			virtual void	generateCodeInternal(FileParsingResult const&	parsingResult,
												 FileGenerationResult&		out_genResult)					noexcept = 0;

			/**
			*	@brief Called just before FileGenerationUnit::generateCodeInternal. Can be used to perform any pre-generation initialization.
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			virtual void	preGenerateCode(FileParsingResult const&	parsingResult,
											FileGenerationResult&		out_genResult)						noexcept;

			/**
			*	@brief Called just after FileGenerationUnit::generateCodeInternal. Can be used to perform any post-generation cleanup.
			*/
			virtual void	postGenerateCode(FileParsingResult const&	parsingResult,
											 FileGenerationResult&		out_genResult)						noexcept;

			/**
			*	@brief Check if file last write time is newer than reference file last write time.
			*			The method will assert if a path is invalid or is not a file.
			* 
			*	@param file				Path to the file to compare.
			*	@param referenceFile	Path to the reference file to compare.
			* 
			*	@return true if file last write time is newer than referenceFile's, else false.
			*/
			bool			isFileNewerThan(fs::path const& file,
											fs::path const& referenceFile)							const	noexcept;

		public:
			/**
			*	@brief Check whether the generated code for a given source file is up-to-date or not.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool	isUpToDate(fs::path const& sourceFile)					const	noexcept = 0;

			/**
			*	@brief Generate code based on the provided parsing result.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			void			generateCode(FileParsingResult const&	parsingResult,
										 FileGenerationResult&		out_genResult)			noexcept;
	};
}