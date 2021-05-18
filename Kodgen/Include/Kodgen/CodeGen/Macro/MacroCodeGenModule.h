/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/CodeGenModule.h"

namespace kodgen
{
	/**
	*	Module used with MacroFileGenUnit class.
	*/
	class MacroCodeGenModule : public CodeGenModule
	{
		public:
			virtual bool generateCode(EntityInfo const&	entity,
									  CodeGenData&		data,
									  std::string&		inout_result,
									  std::string&		out_errorMessage) const noexcept override;
	};
}