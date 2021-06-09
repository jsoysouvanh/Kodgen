/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <memory>		//std::shared_ptr
#include <vector>
#include <type_traits>	//std::is_base_of_v, std::is_copy_constructible_v

#include "Kodgen/Parsing/ParsingResults/FileParsingResult.h"
#include "Kodgen/CodeGen/ETraversalBehaviour.h"
#include "Kodgen/CodeGen/FileGenerationResult.h"
#include "Kodgen/CodeGen/CodeGenEnv.h"
#include "Kodgen/CodeGen/CodeGenUnitSettings.h"
#include "Kodgen/CodeGen/CodeGenModule.h"
#include "Kodgen/Misc/ILogger.h"
#include "Kodgen/Misc/Filesystem.h"
#include "Kodgen/Misc/FundamentalTypes.h"

namespace kodgen
{
	class CodeGenUnit
	{
		private:
			/** Collection of all registered generation modules. */
			std::shared_ptr<std::vector<CodeGenModule const*>>	_generationModules;

			/**
			*	@brief Create the generationModules shared pointer if it is empty.
			* 
			*	@return true if the generation modules has been initialized by the call, else false.
			*/
			bool initializeGenerationModulesIfNecessary()	noexcept;

		protected:
			/** Settings used for code generation. */
			CodeGenUnitSettings const*						settings			= nullptr;

			/**
			*	@brief	Called just before FileGenerationUnit::generateCodeInternal.
			*			Perform all registered modules initialization.
			*			The whole generation process is aborted if the method returns false.
			*			/!\ Overrides should call this base implementation as well /!\
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool				preGenerateCode(FileParsingResult const&	parsingResult,
														CodeGenEnv&					env)								noexcept;

			/**
			*	@brief	Execute the codeGenModule->generateCode method with the given entity and environment.
			*			The method is made virtual pure to let the user control in which string the generated code should be appended.
			*			The implementation is also free to run the module multiple times by altering the environment.
			*
			*	@param codeGenModule	Module calling the generateCode method.
			*	@param entity			Target entity for this code generation pass.
			*	@param env				Generation environment structure.
			* 
			*	@return An EIterationResult instructing how the entity traversal should continue (see the EIterationResult documentation for more info).
			*			The result of the codeGenModule->generateCode() call can be forwarded in most cases to let the module control the flow of the traversal.
			*/
			virtual ETraversalBehaviour	runCodeGenModuleOnEntity(CodeGenModule const&	codeGenModule,
																 EntityInfo const&		entity,
																 CodeGenEnv&			env)							noexcept	= 0;

			/**
			*	@brief	Called just after FileGenerationUnit::generateCodeInternal.
			*			Can be used to perform any post-generation tasks or cleanup.
			*			If the unit needs to write the generated code in files, this is typically here
			*			that files are created and written to.
			*
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param env				Generation environment structure after the preGenerateCode and generateCodeInternal have run.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool				postGenerateCode(FileParsingResult const&	parsingResult,
														 CodeGenEnv&				env)								noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on each parsed entity/registered module pair.
			* 
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param env		Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachEntity(ETraversalBehaviour	(*visitor)(CodeGenModule const&,
																					   EntityInfo const&,
																					   CodeGenUnit&,
																					   CodeGenEnv&),
													  CodeGenEnv&			env)										noexcept;

			/**
			*	@brief	Iterate and execute recursively a visitor function on a namespace and
			*			all its nested entities/registered module pair.
			* 
			*	@param namespace_	Namespace to iterate on.
			*	@param visitor		Visitor function to execute on all traversed entities.
			*	@param env			Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachEntityInNamespace(NamespaceInfo const&	namespace_,
																 ETraversalBehaviour	(*visitor)(CodeGenModule const&,
																								   EntityInfo const&,
																								   CodeGenUnit&,
																								   CodeGenEnv&),
																 CodeGenEnv&			env)							noexcept;

			/**
			*	@brief	Iterate and execute recursively a visitor function on a struct or class and
			*			all its nested entities/registered module pair.
			* 
			*	@param struct_	Struct/class to iterate on.
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param env		Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachEntityInStruct(StructClassInfo const&	struct_,
															  ETraversalBehaviour		(*visitor)(CodeGenModule const&,
																								   EntityInfo const&,
																								   CodeGenUnit&,
																								   CodeGenEnv&),
															  CodeGenEnv&				env)							noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on an enum and all its nested entities.
			* 
			*	@param enum_	Enum to iterate on.
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param env		Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachEntityInEnum(EnumInfo const&		enum_,
															ETraversalBehaviour	(*visitor)(CodeGenModule const&,
																						   EntityInfo const&,
																						   CodeGenUnit&,
																						   CodeGenEnv&),
															CodeGenEnv&			env)									noexcept;

			/**
			*	@brief Check if file last write time is newer than reference file last write time.
			*			The method will assert if a path is invalid or is not a file.
			* 
			*	@param file				Path to the file to compare.
			*	@param referenceFile	Path to the reference file to compare.
			* 
			*	@return true if file last write time is newer than referenceFile's, else false.
			*/
			bool						isFileNewerThan(fs::path const& file,
														fs::path const& referenceFile)							const	noexcept;

		public:
			/** Logger used to issue logs from this CodeGenUnit. */
			ILogger*	logger	= nullptr;

			virtual ~CodeGenUnit() = default;

			/**
			*	@brief	Generate code based on the provided parsing result.
			*			If any of preGenerateCode, generateCodeInternal or postGenerateCode returns false,
			*			the code generation is aborted for this generation unit and false is returned.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param env				Generation environment.
			* 
			*	@return false if the generation process was aborted prematurely because of any error, else true.
			*/
			bool						generateCode(FileParsingResult const&	parsingResult,
													 CodeGenEnv&				env)			noexcept;

			/**
			*	@brief Check whether the generated code for a given source file is up-to-date or not.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool				isUpToDate(fs::path const& sourceFile)			const	noexcept = 0;

			/**
			*	@brief	Check whether all settings are setup correctly for this unit to work.
			*			If output directory path is valid but doesn't exist yet, it is created.
			*			This method is internally called by FileGenerator::generateFiles.
			* 
			*	@return	true if all settings are valid, else false.
			*			Note that the method will return false if the output directory failed to be created (only if it didn't exist).
			*/
			virtual bool				checkSettings()									const	noexcept;

			/**
			*	@brief Add a module to the internal list of generation modules.
			* 
			*	@param generationModule The generation module to add.
			*/
			void						addModule(CodeGenModule const& generationModule)		noexcept;

			/**
			*	@brief Remove a module from the internal list of generation modules.
			* 
			*	@param generationModule The generation module to remove.
			* 
			*	@return true if a module has been successfully removed, else false.
			*/
			bool						removeModule(CodeGenModule const& generationModule)		noexcept;

			/**
			*	@brief Getter for settings field.
			* 
			*	@return settings.
			*/
			CodeGenUnitSettings const*	getSettings()									const	noexcept;
	};
}