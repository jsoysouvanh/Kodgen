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
			addPropertyCodeGen(_getPropertyCodeGen);
			addPropertyCodeGen(_setPropertyCodeGen);
		}

		GetSetCGM(GetSetCGM const&)
		{
			//The copy should use its own PropertyCodeGen instances
			addPropertyCodeGen(_getPropertyCodeGen);
			addPropertyCodeGen(_setPropertyCodeGen);
		}

		virtual ICloneable* clone() const noexcept override
		{
			return new GetSetCGM(*this);
		}
};