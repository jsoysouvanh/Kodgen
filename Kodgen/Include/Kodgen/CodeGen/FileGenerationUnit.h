/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/FileGenerationSettings.h"
#include "Kodgen/CodeGen/FileGenerationResult.h"
#include "Kodgen/CodeGen/CodeGenerationData.h"
#include "Kodgen/Parsing/ParsingResults/FileParsingResult.h"
#include "Kodgen/Misc/ILogger.h"
#include "Kodgen/Misc/Filesystem.h"
#include "Kodgen/Misc/FundamentalTypes.h"

namespace kodgen
{
	class FileGenerationUnit
	{
		//TODO: Delete this
		//FileGenerator can access FileGenerationUnit class to forward logger and settings
		friend class FileGenerator;

		protected:
			enum class EIterationResult
			{
				/**
				*	Recursively traverse the entities contained in the current entity, using
				*	the same visitor and data.
				*/
				Recurse = 0,

				/**
				*	Continues the entities traversal with the next sibling entity without visiting
				*	nested entities.
				*/
				Continue,

				/**
				*	Cancel the iteration on the parent's nested entities and continue with the next parent's sibling.
				*/
				Break,

				/**
				*	Abort completely the entity iteration but make the generateCode method return true (success).
				*/
				AbortWithSuccess,

				/**
				*	Abort completely the entity iteration and make the generateCode method return false (failure).
				*/
				AbortWithFailure
			};

		private:
			/**
			*	TODO
			*/
			EIterationResult foreachEntityInNamespace(NamespaceInfo const&	namespace_,
													  EIterationResult (*visitor)(EntityInfo const&, CodeGenerationData&),
													  CodeGenerationData&	data)											noexcept;

			EIterationResult foreachEntityInStruct(StructClassInfo const&	struct_,
												   EIterationResult (*visitor)(EntityInfo const&, CodeGenerationData&),
												   CodeGenerationData&		data)											noexcept;

			EIterationResult foreachEntityInEnum(EnumInfo const&		enum_,
												 EIterationResult (*visitor)(EntityInfo const&, CodeGenerationData&),
												 CodeGenerationData&	data)												noexcept;

		protected:
			/////TODO: Move this as public field and make the user load generation settings directly in the generation unit?
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
			virtual bool	generateCodeInternal(FileParsingResult const&	parsingResult,
												 FileGenerationResult&		out_genResult)					noexcept = 0;

			/**
			*	@brief Called just before FileGenerationUnit::generateCodeInternal. Can be used to perform any pre-generation initialization.
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			virtual bool		preGenerateCode(FileParsingResult const&	parsingResult,
												FileGenerationResult&		out_genResult)						noexcept;

			/**
			*	@brief Called just after FileGenerationUnit::generateCodeInternal. Can be used to perform any post-generation cleanup.
			*/
			virtual bool		postGenerateCode(FileParsingResult const&	parsingResult,
												 FileGenerationResult&		out_genResult)						noexcept;

			/**
			*	@brief Iterate and execute recursively a function on each parsed entity.
			* 
			*	@TODO
			*/
			EIterationResult	foreachEntity(EIterationResult (*visitor)(EntityInfo const&, CodeGenerationData&),
											  CodeGenerationData& data)											noexcept;

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
			*	@brief	Generate code based on the provided parsing result.
			*			If any of preGenerateCode, generateCodeInternal or postGenerateCode returns false,
			*			the code generation is aborted for this generation unit and false is returned.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			* 
			*	@return false if the generation process was aborted prematurely because of any error, else true.
			*/
			bool			generateCode(FileParsingResult const&	parsingResult,
										 FileGenerationResult&		out_genResult)			noexcept;
	};
}