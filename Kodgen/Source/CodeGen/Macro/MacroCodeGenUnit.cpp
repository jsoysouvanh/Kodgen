#include "Kodgen/CodeGen/Macro/MacroCodeGenUnit.h"

#include "Kodgen/Config.h"
#include "Kodgen/CodeGen/GeneratedFile.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenEnv.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h"
#include "Kodgen/CodeGen/CodeGenHelpers.h"

using namespace kodgen;

bool MacroCodeGenUnit::generateCode(FileParsingResult const& parsingResult) noexcept
{
	MacroCodeGenEnv codeGenEnv;
	return generateCodeInternal(parsingResult, codeGenEnv);
}

ETraversalBehaviour MacroCodeGenUnit::runCodeGenModuleOnEntity(CodeGenModule& codeGenModule, EntityInfo const& entity, CodeGenEnv& env) noexcept
{
	ETraversalBehaviour result		= CodeGenHelpers::leastPrioritizedTraversalBehaviour;
	MacroCodeGenEnv&	macroEnv	= static_cast<MacroCodeGenEnv&>(env);

	//Generate code for each code location
	for (int i = 0u; i < static_cast<int>(ECodeGenLocation::Count); i++)
	{
		macroEnv._codeGenLocation = static_cast<ECodeGenLocation>(i);
		macroEnv._separator = macroEnv._separators[i];

		//Clear the temp string without deallocating underlying memory
		macroEnv._generatedCodeTmp.clear();

		/**
		*	Forward ECodeGenLocation::ClassFooter generation only if the entity is a
		*	struct, class, method or field
		*/
		if (macroEnv._codeGenLocation == ECodeGenLocation::ClassFooter)
		{
			if (!(entity.entityType == EEntityType::Struct || entity.entityType == EEntityType::Class ||
				entity.entityType == EEntityType::Method || entity.entityType == EEntityType::Field))
			{
				continue;
			}
			else
			{
				result = CodeGenHelpers::combineTraversalBehaviours(generateEntityClassFooterCode(codeGenModule, entity, macroEnv), result);
			}
		}
		else
		{
			result = CodeGenHelpers::combineTraversalBehaviours(codeGenModule.generateCode(&entity, env, macroEnv._generatedCodeTmp), result);

			//Append the generated code to the string
			macroEnv._generatedCodePerLocation[i] += macroEnv._generatedCodeTmp;
		}

		//Abort the generation if the current result is AbortWithFailure
		if (result == ETraversalBehaviour::AbortWithFailure)
			return result;
	}

	return result;
}

bool MacroCodeGenUnit::postGenerateCode(CodeGenEnv& env) noexcept
{
	//Create generated header & generated source files
	generateHeaderFile(static_cast<MacroCodeGenEnv&>(env));
	generateSourceFile(static_cast<MacroCodeGenEnv&>(env));

	return true;
}

void MacroCodeGenUnit::generateHeaderFile(MacroCodeGenEnv& env) const noexcept
{
	GeneratedFile generatedHeader(getGeneratedHeaderFilePath(env.getFileParsingResult()->parsedFile), env.getFileParsingResult()->parsedFile);

	MacroCodeGenUnitSettings const* castSettings = static_cast<MacroCodeGenUnitSettings const*>(settings);

	generatedHeader.writeLine("#pragma once\n");

	//Include the entity file
	generatedHeader.writeLine("#include \"" + CodeGenUnitSettings::entityMacrosFilename.string() + "\"");

	//Write header file header code
	generatedHeader.writeLine(std::move(env._generatedCodePerLocation[static_cast<int>(ECodeGenLocation::HeaderFileHeader)]));

	//Write all class footer macros
	for (auto& [structInfo, generatedCode] : env._classFooterGeneratedCode)
	{
		generatedHeader.writeMacro(castSettings->getClassFooterMacro(*structInfo), std::move(generatedCode));
	}

	generatedHeader.writeMacro(castSettings->getHeaderFileFooterMacro(env.getFileParsingResult()->parsedFile),
							   std::move(env._generatedCodePerLocation[static_cast<int>(ECodeGenLocation::HeaderFileFooter)]));
}

void MacroCodeGenUnit::generateSourceFile(MacroCodeGenEnv& env) const noexcept
{
	GeneratedFile generatedSource(getGeneratedSourceFilePath(env.getFileParsingResult()->parsedFile), env.getFileParsingResult()->parsedFile);

	generatedSource.writeLine("#pragma once\n");
	
	//Include the header file
	generatedSource.writeLine("#include \"" + env.getFileParsingResult()->parsedFile.string() + "\"\n");

	generatedSource.writeLine(std::move(env._generatedCodePerLocation[static_cast<int>(ECodeGenLocation::SourceFileHeader)]));
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

ETraversalBehaviour MacroCodeGenUnit::generateEntityClassFooterCode(CodeGenModule& codeGenModule, EntityInfo const& entity, MacroCodeGenEnv& env) noexcept
{
	ETraversalBehaviour result = codeGenModule.generateCode(&entity, env, env._generatedCodeTmp);

	if (result != ETraversalBehaviour::AbortWithFailure)
	{
		//Append the generated code to the relevant string
		if (entity.entityType == EEntityType::Struct || entity.entityType == EEntityType::Class)
		{
			env._classFooterGeneratedCode[&static_cast<StructClassInfo const&>(entity)] += env._generatedCodeTmp;
		}
		else
		{
			assert(entity.outerEntity != nullptr);
			assert(entity.outerEntity->entityType == EEntityType::Struct || entity.outerEntity->entityType == EEntityType::Class);

			env._classFooterGeneratedCode[static_cast<StructClassInfo const*>(entity.outerEntity)] += env._generatedCodeTmp;
		}
	}

	return result;
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