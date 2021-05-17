/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <vector>
#include <string>

namespace kodgen
{
	//Forward declarations
	class EntityInfo;
	class CodeGenerationModule;
	struct CodeGenerationData;

	class CodeGenerationModuleGroup
	{
		private:
			/** Collection of generation modules. */
			std::vector<CodeGenerationModule*>	_generationModules;

		public:
			/**
			*	@brief Add a module to the internal list of generation modules.
			* 
			*	@param generationModule The generation module to add.
			*/
			void addModule(CodeGenerationModule& generationModule)		noexcept;

			/**
			*	@brief Remove a module from the internal list of generation modules.
			* 
			*	@param generationModule The generation module to remove.
			* 
			*	@return true if a module has been successfully removed, else false.
			*/
			bool removeModule(CodeGenerationModule& generationModule)	noexcept;

			/**
			*	@brief	Generate code by executing the generateCode method of each registered generation module.
			* 
			*	@param entity			Entity the code is generated for.
			*	@param data				Code generation data (yes).
			*	@param out_errorMessage	Error message filled if an error happened during code generation.
			* 
			*	@return true if all registered generation modules completed their code generation successfully, else false.
			*			If false is returned, out_errorMessage must be filled with an informative message describing why code generation failed.
			*/
			bool generateCode(EntityInfo const&		entity,
							  CodeGenerationData&	data,
							  std::string&			out_errorMessage)	noexcept;
	};
}