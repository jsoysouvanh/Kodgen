#pragma once

#include <Kodgen/CodeGen/Macro/MacroCodeGenModule.h>

#include "GetPropertyCodeGen.h"
#include "SetPropertyCodeGen.h"

class GetSetCGM : public kodgen::MacroCodeGenModule
{
	private:
		GetPropertyCodeGen	_getPropertyCodeGen;
		SetPropertyCodeGen	_setPropertyCodeGen;

	public:
		GetSetCGM() noexcept
		{
			addPropertyRule(_getPropertyCodeGen);
			addPropertyRule(_setPropertyCodeGen);
		}
};