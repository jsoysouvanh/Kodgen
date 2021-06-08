#include "Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h"

#include <cassert>

using namespace kodgen;

bool MacroPropertyCodeGen::generateCode(EntityInfo const& entity, Property const& property, uint8 propertyIndex, CodeGenEnv& data, std::string& inout_result) const noexcept
{
	MacroCodeGenEnv& macroData = static_cast<MacroCodeGenEnv&>(data);

	//Dispatch code generation call to the right sub-method
	switch (macroData.codeGenLocation)
	{
		case ECodeGenLocation::HeaderFileHeader:
			return preGenerateCode(entity, property, propertyIndex, macroData) && generateHeaderFileHeaderCode(entity, property, propertyIndex, macroData, inout_result);

		case ECodeGenLocation::ClassFooter:
			return generateClassFooterCode(entity, property, propertyIndex, macroData, inout_result);

		case ECodeGenLocation::HeaderFileFooter:
			return generateHeaderFileFooterCode(entity, property, propertyIndex, macroData, inout_result);

		case ECodeGenLocation::SourceFileHeader:
			return generateSourceFileHeaderCode(entity, property, propertyIndex, macroData, inout_result) && postGenerateCode(entity, property, propertyIndex, macroData);

		case ECodeGenLocation::Count:
			//Should never get here
			if (data.logger != nullptr)
			{
				data.logger->log("MacroPropertyCodeGen::generateCode called with ECodeGenLocation::Count location. Abort generation.", ILogger::ELogSeverity::Error);
			}
			return false;
	}

	//Should never reach this point as all cases should be handled by the previous switch statement
	assert(false);
	return false;
}

bool MacroPropertyCodeGen::initialize(CodeGenEnv& env) const noexcept
{
#ifdef RTTI_ENABLED
	return dynamic_cast<MacroCodeGenEnv*>(&env) != nullptr;
#else
	//Can't check dynamic type without RTTI, so return true by default (unsafe since it bypasses all security checks).
	return true;
#endif
}

bool MacroPropertyCodeGen::generateHeaderFileHeaderCode(EntityInfo const& /* entity */, Property const& /* property */, uint8 /* propertyIndex */,
														MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::generateClassFooterCode(EntityInfo const& /* entity */, Property const& /* property */, uint8 /* propertyIndex */,
												   MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::generateHeaderFileFooterCode(EntityInfo const& /* entity */, Property const& /* property */, uint8 /* propertyIndex */,
														MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::generateSourceFileHeaderCode(EntityInfo const& /* entity */, Property const& /* property */, uint8 /* propertyIndex */,
														MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::preGenerateCode(EntityInfo const& /* entity */, Property const& /* property */, uint8 /*propertyIndex*/, CodeGenEnv& /*data*/) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::postGenerateCode(EntityInfo const& /*entity*/, Property const& /*property*/, uint8 /*propertyIndex*/, CodeGenEnv& /*data*/) const noexcept
{
	//Default implementation generates no code
	return true;
}