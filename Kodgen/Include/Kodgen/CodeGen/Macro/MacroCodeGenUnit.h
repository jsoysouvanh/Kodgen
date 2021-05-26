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
	struct	MacroCodeGenData;

	class MacroCodeGenUnit : public CodeGenUnit
	{
		private:
			/**
			*	@brief Handle the whole code generation for a given entity with the provided data.
			* 
			*	@param entity	Entity we generate the code for.
			*	@param data		Generation data (this is some concise documentation).
			* 
			*	@return EIterationResult::Recurse if the traversal completed successfully.
			*			EIterationResult::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			EIterationResult::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			static EIterationResult generateEntityCode(EntityInfo const& entity, CodeGenData& data)	noexcept;

			/**
			*	@brief Handle the code generation for class footer code gen location.
			* 
			*	@param entity	Entity we generate the code for. Must be one of Struct/Class/Field/Method.
			*	@param data		Generation data (this is some concise documentation).
			* 
			*	@return true if the entity class footer code was generated successfully, else false.
			*/
			static bool				generateEntityClassFooterCode(EntityInfo const&	entity,
																  MacroCodeGenData&	data)			noexcept;

		protected:
			/**
			*	@brief	Generate code based on the provided parsing result.
			*			Generated code will be dispatched in 2 different files (1 header, 1 source).
			*
			*	@param parsingResult Result of a file parsing used to generate the new file.
			* 
			*	@return true if the code generation completed successfully without error, else false.
			*/
			virtual bool	generateCodeInternal(FileParsingResult const& parsingResult)			noexcept	override;

			/**
			*	@brief	(Re)generate the header file.
			* 
			*	@param parsingResult	Result of the parsing process.
			*	@param data				Generation data (this is some concise documentation).
			*/
			void			generateHeaderFile(FileParsingResult const& parsingResult,
											   MacroCodeGenData&	data)					const	noexcept;

			/**
			*	@brief	(Re)generate the source file.
			* 
			*	@param parsingResult	Result of the parsing process.
			*	@param data				Generation data (this is some concise documentation)
			*/
			void			generateSourceFile(FileParsingResult const& parsingResult,
											   MacroCodeGenData&	data)					const	noexcept;

			/**
			*	@brief Compute the path of the header file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the header file generated from the provided source file.
			*/
			fs::path	getGeneratedHeaderFilePath(fs::path const& sourceFile)				const	noexcept;

			/**
			*	@brief Compute the path of the source file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the source file generated from the provided source file.
			*/
			fs::path	getGeneratedSourceFilePath(fs::path const& sourceFile)				const	noexcept;

		public:
			/** Pointer to a collection of all generation modules used by this generation unit. */
			CodeGenModuleGroup* codeGenModuleGroup = nullptr;

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
			virtual bool	isUpToDate(fs::path const& sourceFile)	const	noexcept	override;

			/**
			*	@brief	Check whether all settings are setup correctly for this unit to work.
			*			If output directory path is valid but doesn't exist yet, it is created.
			*			If the assigned settings can't be cast to "MacroCodeGenUnitSettings", fail.
			* 
			*	@return	true if all settings are valid, else false.
			*			Note that the method will return false if the output directory failed to be created (only if it didn't exist).
			*/
			virtual bool	checkSettings()							const	noexcept	override;
	};
}