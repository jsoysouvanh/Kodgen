#include "Kodgen/CodeGen/Macro/MacroCodeGenModule.h"

#include "Kodgen/InfoStructures/EntityInfo.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenEnv.h"

#include "Kodgen/CodeGen/CodeGenHelpers.h"

using namespace kodgen;

ETraversalBehaviour MacroCodeGenModule::generateCode(EntityInfo const* entity, CodeGenEnv& data, std::string& inout_result) const noexcept
{
	MacroCodeGenEnv& macroData = static_cast<MacroCodeGenEnv&>(data);

	ETraversalBehaviour result;

	//Dispatch code generation call to the right sub-method
	switch (macroData.codeGenLocation)
	{
		case ECodeGenLocation::HeaderFileHeader:
			//Property code gen are triggered AFTER MacroCodeGenModule::preGenerateCode has been called
			result = preGenerateCode(entity, macroData);
			result = CodeGenHelpers::combineTraversalBehaviours(result, generateHeaderFileHeaderCode(entity, macroData, inout_result));

			//CodeGenModule::generateCode runs eligible property code generators if any
			return CodeGenHelpers::combineTraversalBehaviours(result, CodeGenModule::generateCode(entity, data, inout_result));

		case ECodeGenLocation::ClassFooter:
			result = generateClassFooterCode(entity, macroData, inout_result);

			//CodeGenModule::generateCode runs eligible property code generators if any
			return CodeGenHelpers::combineTraversalBehaviours(result, CodeGenModule::generateCode(entity, data, inout_result));

		case ECodeGenLocation::HeaderFileFooter:
			result = generateHeaderFileFooterCode(entity, macroData, inout_result);

			//CodeGenModule::generateCode runs eligible property code generators if any
			return CodeGenHelpers::combineTraversalBehaviours(result, CodeGenModule::generateCode(entity, data, inout_result));

		case ECodeGenLocation::SourceFileHeader:
			result = generateSourceFileHeaderCode(entity, macroData, inout_result);
			
			//Property code gen are triggered BEFORE calling MacroCodeGenModule::postGenerateCode
			result = CodeGenHelpers::combineTraversalBehaviours(result, CodeGenModule::generateCode(entity, data, inout_result));

			return CodeGenHelpers::combineTraversalBehaviours(result, postGenerateCode(entity, macroData));

		case ECodeGenLocation::Count:
			//Should never get here
			if (data.logger != nullptr)
			{
				data.logger->log("MacroPropertyCodeGen::generateCode called with ECodeGenLocation::Count location. Abort generation.", ILogger::ELogSeverity::Error);
			}
			return ETraversalBehaviour::AbortWithFailure;
	}
	
	//Should never reach this point as all cases should be handled by the previous switch statement
	assert(false);

	return ETraversalBehaviour::AbortWithFailure;
}

ETraversalBehaviour MacroCodeGenModule::generateHeaderFileHeaderCode(EntityInfo const* /* entity */, MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

ETraversalBehaviour MacroCodeGenModule::generateClassFooterCode(EntityInfo const* /* entity */, MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

ETraversalBehaviour MacroCodeGenModule::generateHeaderFileFooterCode(EntityInfo const* /* entity */, MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

ETraversalBehaviour MacroCodeGenModule::generateSourceFileHeaderCode(EntityInfo const* /* entity */, MacroCodeGenEnv& /* data */, std::string& /* inout_result */) const noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

ETraversalBehaviour MacroCodeGenModule::preGenerateCode(EntityInfo const* /* entity */, CodeGenEnv& /*data*/) const noexcept
{
	//Default implementation does nothing
	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour MacroCodeGenModule::postGenerateCode(EntityInfo const* /*entity*/, CodeGenEnv& /*data*/) const noexcept
{
	//Default implementation does nothing
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}