#pragma once

#include <Kodgen/CodeGen/CodeGenModule.h>

#include "GetPropertyCodeGen.h"
#include "SetPropertyCodeGen.h"

class GetSetCGM : public kodgen::CodeGenModule
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