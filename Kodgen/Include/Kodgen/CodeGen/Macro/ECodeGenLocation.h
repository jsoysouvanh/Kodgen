/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

namespace kodgen
{
	enum class ECodeGenLocation
	{
		/**
		*	Code will be generated at the very top of the generated header file (without macro).
		*	The code is then injected as soon as the generated header file is included in any other file.
		*/
		HeaderFileHeader = 0,

		/**
		*	Code will be inserted in a macro generated for each tagged struct/class.
		*	The generated macro name is customizable using the MacroCodeGenUnitSettings::setClassFooterMacroPattern method.
		*/
		ClassFooter,

		/**
		*	Code will be inserted in a macro to add at the very bottom of parsed header files.
		*	The generated macro name is customizable using the MacroCodeGenUnitSettings::setHeaderFileFooterMacroPattern method.
		*/
		HeaderFileFooter,

		/**
		*	Code will be generated at the very top of the generated source file (without macro).
		*	The code is then injected as soon as the generated source file is included in any other file.
		*/
		SourceFileHeader,

		/**
		*	Internal use only.
		*/
		Count
	};
}