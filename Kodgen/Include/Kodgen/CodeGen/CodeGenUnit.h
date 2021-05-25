/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/FileGenerationResult.h"
#include "Kodgen/Parsing/ParsingResults/FileParsingResult.h"
#include "Kodgen/CodeGen/CodeGenData.h"
#include "Kodgen/CodeGen/CodeGenUnitSettings.h"
#include "Kodgen/Misc/ILogger.h"
#include "Kodgen/Misc/Filesystem.h"
#include "Kodgen/Misc/FundamentalTypes.h"

namespace kodgen
{
	class CodeGenUnit
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
				*	Cancel the traversal on the siblings of the same entity type and resume
				*	it with the next sibling of a different type.
				*/
				Break,

				/**
				*	Abort the whole traversal but make the generateCode method return true (success).
				*/
				AbortWithSuccess,

				/**
				*	Abort the whole traversal and make the generateCode method return false (failure).
				*/
				AbortWithFailure
			};

		protected:
			/**
			*	@brief	Generate code based on the provided parsing result.
			*			It is up to this method to create files to write to or not.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			virtual bool		generateCodeInternal(FileParsingResult const&	parsingResult,
													 FileGenerationResult&		out_genResult)									noexcept = 0;

			/**
			*	@brief	Called just before FileGenerationUnit::generateCodeInternal.
			*			Can be used to perform any pre-generation initialization.
			*			The whole generation process is aborted if the method returns false, so
			*			validity checks can also be performed during this step.
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool		preGenerateCode(FileParsingResult const&	parsingResult,
												FileGenerationResult&		out_genResult)										noexcept;

			/**
			*	@brief	Called just after FileGenerationUnit::generateCodeInternal.
			*			Can be used to perform any post-generation cleanup.
			*
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool		postGenerateCode(FileParsingResult const&	parsingResult,
												 FileGenerationResult&		out_genResult)										noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on each parsed entity.
			* 
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param data		User defined generation data structure.
			* 
			*	@return EIterationResult::Recurse if the traversal completed successfully.
			*			EIterationResult::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			EIterationResult::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			EIterationResult	foreachEntity(EIterationResult (*visitor)(EntityInfo const&, CodeGenData&),
											  CodeGenData& data)															noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on a namespace and all its nested entities.
			* 
			*	@param namespace_	Namespace to iterate on.
			*	@param visitor		Visitor function to execute on all traversed entities.
			*	@param data			User defined generation data structure.
			* 
			*	@return EIterationResult::Recurse if the traversal completed successfully.
			*			EIterationResult::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			EIterationResult::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			EIterationResult	foreachEntityInNamespace(NamespaceInfo const&	namespace_,
														 EIterationResult (*visitor)(EntityInfo const&, CodeGenData&),
														 CodeGenData&	data)											noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on a struct or class and all its nested entities.
			* 
			*	@param struct_	Struct/class to iterate on.
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param data		User defined generation data structure.
			* 
			*	@return EIterationResult::Recurse if the traversal completed successfully.
			*			EIterationResult::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			EIterationResult::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			EIterationResult	foreachEntityInStruct(StructClassInfo const&	struct_,
													  EIterationResult (*visitor)(EntityInfo const&, CodeGenData&),
													  CodeGenData&		data)											noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on an enum and all its nested entities.
			* 
			*	@param enum_	Enum to iterate on.
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param data		User defined generation data structure.
			* 
			*	@return EIterationResult::Recurse if the traversal completed successfully.
			*			EIterationResult::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			EIterationResult::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			EIterationResult	foreachEntityInEnum(EnumInfo const&		enum_,
													EIterationResult (*visitor)(EntityInfo const&, CodeGenData&),
													CodeGenData&	data)													noexcept;

			/**
			*	@brief Check if file last write time is newer than reference file last write time.
			*			The method will assert if a path is invalid or is not a file.
			* 
			*	@param file				Path to the file to compare.
			*	@param referenceFile	Path to the reference file to compare.
			* 
			*	@return true if file last write time is newer than referenceFile's, else false.
			*/
			bool				isFileNewerThan(fs::path const& file,
												fs::path const& referenceFile)											const	noexcept;

		public:
			/** Logger used to issue logs from the FileGenerationUnit. */
			ILogger*					logger		= nullptr;

			/** Generation settings. */
			CodeGenUnitSettings const*	settings	= nullptr;

			/**
			*	@brief Check whether the generated code for a given source file is up-to-date or not.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool	isUpToDate(fs::path const& sourceFile)					const	noexcept = 0;

			/**
			*	@brief	Check whether all settings are setup correctly for this unit to work.
			*			If output directory path is valid but doesn't exist yet, it is created.
			* 
			*	@return	true if all settings are valid, else false.
			*			Note that the method will return false if the output directory failed to be created (only if it didn't exist).
			*/
			virtual bool	checkSettings()											const	noexcept;

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