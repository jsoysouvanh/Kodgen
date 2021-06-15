/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>
#include <array>
#include <unordered_map>

#include "Kodgen/CodeGen/CodeGenEnv.h"
#include "Kodgen/CodeGen/Macro/ECodeGenLocation.h"

namespace kodgen
{
	//Forward declaration
	class MacroCodeGenUnit;

	class MacroCodeGenEnv : public CodeGenEnv
	{
		//MacroCodeGenUnit is the only class allowed to set the private fields directly.
		//Other classes must access the fields through getters.
		friend MacroCodeGenUnit;

		private:
			/** Separator used for each code location. */
			static inline std::array<std::string, static_cast<size_t>(ECodeGenLocation::Count)> const	_separators
			{
				"\n",	//HeaderFileHeader is not wrapped inside a macro, so can use \n without breaking the code
				"\\\n",	//ClassFooter is wrapped in a macro so must use \ to keep multiline generated code valid
				"\\\n",	//HeaderFileFooter is wrapped in a macro so must use \ to keep multiline generated code valid
				"\n"	//SourceFileHeader is not wrapped in a macro, so can use \n without breaking the code
			};

			/** Temporary string used internally by the MacroCodeGenUnit to avoid memory reallocations for each entity iteration. */
			std::string																_generatedCodeTmp;

			/** Array containing the generated code per location. */
			std::array<std::string, static_cast<size_t>(ECodeGenLocation::Count)>	_generatedCodePerLocation;

			/** Map containing the class footer generated code for each struct/class. */
			std::unordered_map<StructClassInfo const*, std::string>					_classFooterGeneratedCode;

			/** Location the code should be generated in. */
			ECodeGenLocation														_codeGenLocation	= ECodeGenLocation::Count;

			/** Separator to use to split the generated code. */
			std::string																_separator;

		public:
			virtual ~MacroCodeGenEnv() = default;

			/**
			*	@brief Getter for field _codeGenLocation.
			* 
			*	@return _codeGenLocation.
			*/
			inline ECodeGenLocation		getCodeGenLocation()	const	noexcept;

			/**
			*	@brief Getter for field _separator.
			* 
			*	@return _separator.
			*/
			inline std::string const&	getSeparator()			const	noexcept;
	};

	#include "Kodgen/CodeGen/Macro/MacroCodeGenEnv.inl"
}