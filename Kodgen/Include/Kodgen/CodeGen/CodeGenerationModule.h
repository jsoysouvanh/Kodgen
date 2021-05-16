/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>
#include <vector>

#include "Kodgen/Parsing/ParsingResults/FileParsingResult.h"
#include "Kodgen/Properties/PropertyRule.h"

namespace kodgen
{
	//TODO: Add module dependency functionnality?
	class CodeGenerationModule
	{
		protected:
			/** Collection of all property rules attached to this module. */
			std::vector<PropertyRule const*> propertyRules;

		public:
			struct Data
			{
				/** Entity we generate the code for. */
				EntityInfo const&			entity;

				/** Whole parsing result. */
				FileParsingResult const&	parsingResult;

				//TODO: Add FileGenerationSettings?
			};

			/**
			*	Number defining in which order this module will generate code compared to other modules.
			*	Modules with a lower generationOrder will execute first, and modules a high generationOrder will execute last.
			*	Modules having the same generationOrder value will execute in an undefined order.
			*/
			int32	generationOrder	= 0;

			/**
			*	@brief Generate code using the provided data as input.
			* 
			*	@param data				Data provided by the FileGenerationUnit. You can cast data to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param out_errorMessage	Optional error message to fill if something unexpected happens during code generation.
			* 
			*	@return The generated code as a string.
			*/
			virtual std::string generateCode(CodeGenerationModule::Data&	data,
											 std::string&					out_errorMessage) const noexcept = 0;
	};
}