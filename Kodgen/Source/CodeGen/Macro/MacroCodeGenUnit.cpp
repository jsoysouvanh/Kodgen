#include "Kodgen/CodeGen/Macro/MacroCodeGenUnit.h"

#include "Kodgen/Config.h"
#include "Kodgen/CodeGen/GeneratedFile.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenEnv.h"
#include "Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h"
#include "Kodgen/CodeGen/CodeGenHelpers.h"

using namespace kodgen;

bool MacroCodeGenUnit::preGenerateCode(FileParsingResult const& parsingResult, CodeGenEnv& env) noexcept
{
#ifdef RTTI_ENABLED
	 MacroCodeGenEnv* macroEnv = dynamic_cast<MacroCodeGenEnv*>(&env);
#else
	//unsafe
	 MacroCodeGenEnv* macroEnv = static_cast<MacroCodeGenEnv*>(&env);
#endif

	if (macroEnv != nullptr)
	{
		//Setup macro env
		macroEnv->parsingResult = &parsingResult;	//If a crash happens here, means RTTI was disable and the provided env is not a MacroCodeGenEnv
		macroEnv->logger		= logger;

		return CodeGenUnit::preGenerateCode(parsingResult, env);
	}

	if (logger != nullptr)
	{
		logger->log("The provided CodeGenEnv must be castable to MacroCodeGenEnv to be used with a MacroCodeGenUnit.", ILogger::ELogSeverity::Error);
	}

	return false;
}

ETraversalBehaviour MacroCodeGenUnit::runCodeGenModuleOnEntity(CodeGenModule const& codeGenModule, EntityInfo const& entity, CodeGenEnv& env) noexcept
{
	ETraversalBehaviour result = CodeGenHelpers::leastPrioritizedTraversalBehaviour;

	//Data MUST be a MacroCodeGenEnv or derived for this generation unit to work
	MacroCodeGenEnv& macroEnv = static_cast<MacroCodeGenEnv&>(env);

	//Generate code for each code location
	for (int i = 0u; i < static_cast<int>(ECodeGenLocation::Count); i++)
	{
		macroEnv.codeGenLocation = static_cast<ECodeGenLocation>(i);
		macroEnv.separator = macroEnv._separators[i];

		//Clear the temp string without deallocating underlying memory
		macroEnv._generatedCodeTmp.clear();

		/**
		*	Forward ECodeGenLocation::ClassFooter generation only if the entity is a
		*	struct, class, method or field
		*/
		if (macroEnv.codeGenLocation == ECodeGenLocation::ClassFooter)
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

bool MacroCodeGenUnit::postGenerateCode(FileParsingResult const& parsingResult, CodeGenEnv& env) noexcept
{
	//Create generated header & generated source files
	generateHeaderFile(parsingResult, static_cast<MacroCodeGenEnv&>(env));
	generateSourceFile(parsingResult, static_cast<MacroCodeGenEnv&>(env));

	return true;
}

void MacroCodeGenUnit::generateHeaderFile(FileParsingResult const& parsingResult, MacroCodeGenEnv& env) const noexcept
{
	GeneratedFile generatedHeader(getGeneratedHeaderFilePath(parsingResult.parsedFile), parsingResult.parsedFile);

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

	generatedHeader.writeMacro(castSettings->getHeaderFileFooterMacro(parsingResult.parsedFile), std::move(env._generatedCodePerLocation[static_cast<int>(ECodeGenLocation::HeaderFileFooter)]));
}

void MacroCodeGenUnit::generateSourceFile(FileParsingResult const& parsingResult, MacroCodeGenEnv& env) const noexcept
{
	GeneratedFile generatedSource(getGeneratedSourceFilePath(parsingResult.parsedFile), parsingResult.parsedFile);

	generatedSource.writeLine("#pragma once\n");
	
	//Include the header file
	generatedSource.writeLine("#include \"" + parsingResult.parsedFile.string() + "\"\n");

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

ETraversalBehaviour MacroCodeGenUnit::generateEntityClassFooterCode(CodeGenModule const& codeGenModule, EntityInfo const& entity, MacroCodeGenEnv& env) noexcept
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