#include "Kodgen/CodeGen/Macro/MacroCodeGenUnit.h"

#include "Kodgen/Config.h"
#include "Kodgen/CodeGen/CodeGenModuleGroup.h"
#include "Kodgen/CodeGen/GeneratedFile.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenData.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h"

using namespace kodgen;

bool MacroCodeGenUnit::generateCodeInternal(FileParsingResult const& parsingResult) noexcept
{
	//TODO: Move MacroCodeGenData creation to another method, then forwarded to this method through args
	MacroCodeGenData data;
	
	data.parsingResult		= &parsingResult;
	data.logger				= logger;
	data.codeGenModuleGroup = codeGenModuleGroup;

	EIterationResult result = foreachEntity(&MacroCodeGenUnit::generateEntityCode, data);

	if (result != EIterationResult::AbortWithFailure)
	{
		//Create generated header & generated source files
		generateHeaderFile(parsingResult, data);
		generateSourceFile(parsingResult, data);

		return true;
	}
	
	return false;
}

void MacroCodeGenUnit::generateHeaderFile(FileParsingResult const& parsingResult, MacroCodeGenData& data) const noexcept
{
	GeneratedFile generatedHeader(getGeneratedHeaderFilePath(parsingResult.parsedFile), parsingResult.parsedFile);

	MacroCodeGenUnitSettings const* castSettings = static_cast<MacroCodeGenUnitSettings const*>(settings);

	generatedHeader.writeLine("#pragma once\n");

	//Include the entity file
	generatedHeader.writeLine("#include \"" + CodeGenUnitSettings::entityMacrosFilename.string() + "\"");

	//Write header file header code
	generatedHeader.writeLine(std::move(data._generatedCodePerLocation[static_cast<int>(ECodeGenLocation::HeaderFileHeader)]));

	//Write all class footer macros
	for (auto& [structInfo, generatedCode] : data._classFooterGeneratedCode)
	{
		generatedHeader.writeMacro(castSettings->getClassFooterMacro(*structInfo), std::move(generatedCode));
	}

	generatedHeader.writeMacro(castSettings->getHeaderFileFooterMacro(parsingResult.parsedFile), std::move(data._generatedCodePerLocation[static_cast<int>(ECodeGenLocation::HeaderFileFooter)]));
}

void MacroCodeGenUnit::generateSourceFile(FileParsingResult const& parsingResult, MacroCodeGenData& data) const noexcept
{
	GeneratedFile generatedSource(getGeneratedSourceFilePath(parsingResult.parsedFile), parsingResult.parsedFile);

	generatedSource.writeLine("#pragma once\n");
	
	//Include the header file
	generatedSource.writeLine("#include \"" + parsingResult.parsedFile.string() + "\"\n");

	generatedSource.writeLine(std::move(data._generatedCodePerLocation[static_cast<int>(ECodeGenLocation::SourceFileHeader)]));
}

bool MacroCodeGenUnit::isUpToDate(fs::path const& sourceFile) const noexcept
{
	fs::path generatedHeaderPath = getGeneratedHeaderFilePath(sourceFile);

	//If the generated header doesn't exist, create it and return false
	if (!fs::exists(generatedHeaderPath))
	{
		GeneratedFile generatedHeader(fs::path(generatedHeaderPath), sourceFile);
	}
	else if (isFileNewerThan(generatedHeaderPath, sourceFile))
	{
		fs::path generatedSource = getGeneratedSourceFilePath(sourceFile);

		return fs::exists(generatedSource) && isFileNewerThan(generatedSource, sourceFile);
	}

	return false;
}

MacroCodeGenUnit::EIterationResult MacroCodeGenUnit::generateEntityCode(EntityInfo const& entity, CodeGenData& data) noexcept
{
	//Data MUST be a MacroCodeGenData or derived for this generation unit to work
	MacroCodeGenData& macroData = static_cast<MacroCodeGenData&>(data);

	if (macroData.codeGenModuleGroup != nullptr)
	{
		//Generate code for each code location
		for (int i = 0u; i < static_cast<int>(ECodeGenLocation::Count); i++)
		{
			macroData.codeGenLocation = static_cast<ECodeGenLocation>(i);
			macroData.separator = macroData._separators[i];

			//Clear the temp string without deallocating underlying memory
			macroData._generatedCodeTmp.clear();

			/**
			*	Forward ECodeGenLocation::ClassFooter generation only if the entity is a
			*	struct, class, method or field
			*/
			if (macroData.codeGenLocation == ECodeGenLocation::ClassFooter)
			{
				if (!(entity.entityType == EEntityType::Struct || entity.entityType == EEntityType::Class ||
					  entity.entityType == EEntityType::Method || entity.entityType == EEntityType::Field))
				{
					continue;
				}
				else if (!generateEntityClassFooterCode(entity, macroData))
				{
					return EIterationResult::AbortWithFailure;
				}
			}
			else
			{
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
	}

	return EIterationResult::Recurse;
}

bool MacroCodeGenUnit::generateEntityClassFooterCode(EntityInfo const& entity, MacroCodeGenData& data) noexcept
{
	if (data.codeGenModuleGroup->generateCode(&entity, data, data._generatedCodeTmp))
	{
		//Append the generated code to the relevant string
		if (entity.entityType == EEntityType::Struct || entity.entityType == EEntityType::Class)
		{
			data._classFooterGeneratedCode[&static_cast<StructClassInfo const&>(entity)] += data._generatedCodeTmp;
		}
		else
		{
			assert(entity.outerEntity != nullptr);
			assert(entity.outerEntity->entityType == EEntityType::Struct || entity.outerEntity->entityType == EEntityType::Class);

			data._classFooterGeneratedCode[static_cast<StructClassInfo const*>(entity.outerEntity)] += data._generatedCodeTmp;
		}

		return true;
	}
	else
	{
		return false;
	}
}

fs::path MacroCodeGenUnit::getGeneratedHeaderFilePath(fs::path const& sourceFile) const noexcept
{
	return settings->getOutputDirectory() / static_cast<MacroCodeGenUnitSettings const*>(settings)->getGeneratedHeaderFileName(sourceFile);
}

fs::path MacroCodeGenUnit::getGeneratedSourceFilePath(fs::path const& sourceFile) const noexcept
{
	return settings->getOutputDirectory() / static_cast<MacroCodeGenUnitSettings const*>(settings)->getGeneratedSourceFileName(sourceFile);
}

void MacroCodeGenUnit::setSettings(MacroCodeGenUnitSettings const* cguSettings) noexcept
{
	settings = cguSettings;
}