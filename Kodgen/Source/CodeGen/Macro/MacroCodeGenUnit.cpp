#include "Kodgen/CodeGen/Macro/MacroCodeGenUnit.h"

#include "Kodgen/CodeGen/CodeGenModuleGroup.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenData.h"

using namespace kodgen;

bool MacroCodeGenUnit::generateCodeInternal(FileParsingResult const& parsingResult, FileGenerationResult& /* out_genResult */) noexcept
{
	MacroCodeGenData data(parsingResult, logger, codeGenModuleGroup);

	EIterationResult result = foreachEntity(&MacroCodeGenUnit::generateEntityCode, data);

	//TODO: Generate files and process result
	for (std::string const& s : data._generatedCodePerLocation)
	{
		logger->log(s, ILogger::ELogSeverity::Info);
	}

	return result != EIterationResult::AbortWithFailure;
}

bool MacroCodeGenUnit::isUpToDate(fs::path const& /* sourceFile */) const noexcept
{
	//TODO: True if both header generated and cpp generated are newer than source file
	return false;
}

MacroCodeGenUnit::EIterationResult MacroCodeGenUnit::generateEntityCode(EntityInfo const& entity, CodeGenData& data) noexcept
{
	//Data MUST be a MacroCodeGenData or derived for this generation unit to work
	MacroCodeGenData& macroData = static_cast<MacroCodeGenData&>(data);

	if (macroData.codeGenModuleGroup != nullptr)
	{
		//Generate code for each code location
		for (int i = 0; i < static_cast<uint32>(ECodeGenLocation::Count); i++)
		{
			macroData.codeGenLocation = static_cast<ECodeGenLocation>(i);
			macroData.separator = macroData._separators[i];

			/**
			*	Forward ECodeGenLocation::ClassFooter generation only if the entity is a
			*	struct, class, method or field
			*/
			if (macroData.codeGenLocation == ECodeGenLocation::ClassFooter &&
				!(entity.entityType == EEntityType::Struct || entity.entityType == EEntityType::Class ||
				  entity.entityType == EEntityType::Method || entity.entityType == EEntityType::Field))
				continue;

			//Clear the temp string without deallocating underlying memory
			macroData._generatedCodeTmp.clear();

			if (macroData.codeGenModuleGroup->generateCode(&entity, data, macroData._generatedCodeTmp))
			{
				//Append the generated code to the string
				macroData._generatedCodePerLocation[i] += macroData._generatedCodeTmp;
			}
			else
			{
				return EIterationResult::AbortWithFailure;
			}
		}
	}

	return EIterationResult::Recurse;
}