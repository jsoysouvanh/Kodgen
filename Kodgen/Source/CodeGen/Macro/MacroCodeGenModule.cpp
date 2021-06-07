#include "Kodgen/CodeGen/Macro/MacroCodeGenModule.h"

#include "Kodgen/InfoStructures/EntityInfo.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenData.h"

using namespace kodgen;

bool MacroCodeGenModule::generateCode(EntityInfo const* entity, CodeGenData& data, std::string& inout_result) const noexcept
{
	MacroCodeGenData& macroData = static_cast<MacroCodeGenData&>(data);

	//Dispatch code generation call to the right sub-method
	switch (macroData.codeGenLocation)
	{
		case ECodeGenLocation::HeaderFileHeader:
			//Property code gen are triggered AFTER MacroCodeGenModule::preGenerateCode has been called
			return preGenerateCode(entity, macroData) && generateHeaderFileHeaderCode(entity, macroData, inout_result) && CodeGenModule::generateCode(entity, data, inout_result);

		case ECodeGenLocation::ClassFooter:
			return generateClassFooterCode(entity, macroData, inout_result) && CodeGenModule::generateCode(entity, data, inout_result);

		case ECodeGenLocation::HeaderFileFooter:
			return generateHeaderFileFooterCode(entity, macroData, inout_result) && CodeGenModule::generateCode(entity, data, inout_result);

		case ECodeGenLocation::SourceFileHeader:
			//Property code gen are triggered BEFORE calling MacroCodeGenModule::postGenerateCode
			return generateSourceFileHeaderCode(entity, macroData, inout_result) && CodeGenModule::generateCode(entity, data, inout_result) && postGenerateCode(entity, macroData);

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

bool MacroCodeGenModule::generateHeaderFileHeaderCode(EntityInfo const* /* entity */, MacroCodeGenData& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenModule::generateClassFooterCode(EntityInfo const* /* entity */, MacroCodeGenData& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenModule::generateHeaderFileFooterCode(EntityInfo const* /* entity */, MacroCodeGenData& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenModule::generateSourceFileHeaderCode(EntityInfo const* /* entity */, MacroCodeGenData& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenModule::preGenerateCode(EntityInfo const* /* entity */, CodeGenData& /*data*/) const noexcept
{
	//Default implementation does nothing
	return true;
}

bool MacroCodeGenModule::postGenerateCode(EntityInfo const* /*entity*/, CodeGenData& /*data*/) const noexcept
{
	//Default implementation does nothing
	return true;
}