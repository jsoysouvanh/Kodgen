/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <unordered_map>

#include "Kodgen/CodeGen/FileGenerationSettings.h"
#include "Kodgen/CodeGen/GeneratedCodeTemplate.h"
#include "Kodgen/CodeGen/FileGenerationResult.h"
#include "Kodgen/Parsing/FileParser.h"
#include "Kodgen/CodeGen/GeneratedFile.h"
#include "Kodgen/Misc/ILogger.h"

namespace kodgen
{
	class FileGenerationUnit
	{
		//FileGenerator can access FileGenerationUnit class to forward logger and settings
		friend class FileGenerator;

		private:
			/**
			*	
			*/
			/*void					writeFileContent(GeneratedFile&			generatedFile,
													 FileParsingResult&		parsingResult,
													 FileGenerationResult&	out_genResult)					noexcept;*/

			/**
			*	@brief Retrieve the appropriate generated code template to use for a given entity.
			*
			*	@param entityInfo	Information on a parsed entity.
			*
			*	@return The appropriate generated code template if found, else nullptr.
			*/
			//GeneratedCodeTemplate*	getEntityGeneratedCodeTemplate(EntityInfo const& entityInfo)	const	noexcept;

		protected:
			/** Logger used to issue logs from the FileGenerationUnit. */
			ILogger*						logger		= nullptr;

			/** Generation settings. */
			FileGenerationSettings const*	settings	= nullptr;

			/**
			*	@brief Called just before FileGenerationUnit::generateCodeInternal. Can be used to perform any pre-generation initialization.
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			virtual void	preGenerateCode(FileParsingResult const&	parsingResult,
											FileGenerationResult&		out_genResult)						noexcept;

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
			*	@brief Called just after FileGenerationUnit::generateCodeInternal. Can be used to perform any post-generation cleanup.
			*/
			virtual void	postGenerateCode(FileParsingResult const&	parsingResult,
											 FileGenerationResult&		out_genResult)						noexcept;

			///**
			//*	@brief Write a header into the provided file.
			//*
			//*	@param file Reference to the generated file.
			//*	@param parsingResult Structure containing info about the parsed file.
			//*/
			//virtual void	writeHeader(GeneratedFile&				file,
			//							FileParsingResult const&	parsingResult)					const	noexcept;

			///**
			//*	@brief Write a footer into the provided file.
			//*
			//*	@param file Reference to the generated file.
			//*	@param parsingResult Structure containing info about the parsed file.
			//*/
			//virtual void	writeFooter(GeneratedFile&				file,
			//							FileParsingResult const&	parsingResult)					const	noexcept;

			///**
			//*	@brief Generate code for entityInfo in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param entityInfo		Entity we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeEntityToFile(GeneratedFile&			generatedFile,
			//								  EntityInfo&				entityInfo,
			//								  FileParsingResult const&	parsingResult,
			//								  FileGenerationResult&		out_genResult)						noexcept;

			///**
			//*	@brief Generate code for namespaceInfo in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param namespaceInfo	Namespace we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeNamespaceToFile(GeneratedFile&				generatedFile,
			//									 EntityInfo&				namespaceInfo,
			//									 FileParsingResult const&	parsingResult,
			//									 FileGenerationResult&		out_genResult)					noexcept;

			///**
			//*	@brief Generate code for structClassInfo in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param structClassInfo	Struct/class we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeStructOrClassToFile(GeneratedFile&				generatedFile,
			//										 EntityInfo&				structClassInfo,
			//										 FileParsingResult const&	parsingResult,
			//										 FileGenerationResult&		out_genResult)				noexcept;

			///**
			//*	@brief Generate code for nestedStructClassInfo in generatedFile.
			//*
			//*	@param generatedFile			Generated file to write in.
			//*	@param nestedStructClassInfo	Nested struct/class we generate the code from.
			//*	@param parsingResult			Result of the file parsing.
			//*	@param out_genResult			Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeNestedStructOrClassToFile(GeneratedFile&			generatedFile,
			//											   EntityInfo&				nestedStructClassInfo,
			//											   FileParsingResult const&	parsingResult,
			//											   FileGenerationResult&	out_genResult)			noexcept;

			///**
			//*	@brief Generate code for enumInfo in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param enumInfo			Enum we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeEnumToFile(GeneratedFile&				generatedFile,
			//								EntityInfo&					enumInfo,
			//								FileParsingResult const&	parsingResult,
			//								FileGenerationResult&		out_genResult)						noexcept;

			///**
			//*	@brief Generate code for enumValueInfo in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param enumValueInfo	Enum we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeEnumValueToFile(GeneratedFile&				generatedFile,
			//									 EntityInfo&				enumValueInfo,
			//									 FileParsingResult const&	parsingResult,
			//									 FileGenerationResult&		out_genResult)					noexcept;

			///**
			//*	@brief Generate code for a variable in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param variableInfo		Variable we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeVariableToFile(GeneratedFile&				generatedFile,
			//									EntityInfo&					variableInfo,
			//									FileParsingResult const&	parsingResult,
			//									FileGenerationResult&		out_genResult)					noexcept;

			///**
			//*	@brief Generate code for fieldInfo in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param fieldInfo		Field we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeFieldToFile(GeneratedFile&				generatedFile,
			//								 EntityInfo&				fieldInfo,
			//								 FileParsingResult const&	parsingResult,
			//								 FileGenerationResult&		out_genResult)						noexcept;

			///**
			//*	@brief Generate code for a (non-member) function in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param functionInfo		Function we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeFunctionToFile(GeneratedFile&				generatedFile,
			//									EntityInfo&					functionInfo,
			//									FileParsingResult const&	parsingResult,
			//									FileGenerationResult&		out_genResult)					noexcept;

			///**
			//*	@brief Generate code for methodInfo in generatedFile.
			//*
			//*	@param generatedFile	Generated file to write in.
			//*	@param methodInfo		Method we generate the code from.
			//*	@param parsingResult	Result of the file parsing.
			//*	@param out_genResult	Reference to the generation result to fill during file generation.
			//*
			//*	@return true if no generation happened (no code template) or if no error happened during generation, false otherwise.
			//*/
			//virtual bool	writeMethodToFile(GeneratedFile&			generatedFile,
			//								  EntityInfo&				methodInfo,
			//								  FileParsingResult const&	parsingResult,
			//								  FileGenerationResult&		out_genResult)						noexcept;

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
			FileGenerationUnit()							= default;
			FileGenerationUnit(FileGenerationUnit const&)	= default;
			FileGenerationUnit(FileGenerationUnit&&)		= default;
			virtual ~FileGenerationUnit()					= default;

			/**
			*	@brief Generate code based on the provided parsing result.
			*
			*	@param parsingResult	Result of a file parsing used to generate the new file.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			void	generateCode(FileParsingResult const&	parsingResult,
								 FileGenerationResult&		out_genResult)	noexcept;

			/**
			*	@brief Check whether the generated code for a given source file is up-to-date or not.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool	isUpToDate(fs::path const& sourceFile)	const = 0;
	};
}