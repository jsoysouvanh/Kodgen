#include <iostream>

#include <Kodgen/Misc/Filesystem.h>
#include <Kodgen/Misc/DefaultLogger.h>
#include <Kodgen/CodeGen/FileGenerator.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnit.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h>
#include <Kodgen/CodeGen/CodeGenModuleGroup.h>
#include <Kodgen/CodeGen/FileGeneratorSettings.h>

#include "CppPropsParserFactory.h"
#include "GetSetCGM.h"

void initGenerationSettings(fs::path const& workingDirectory, kodgen::FileGeneratorSettings& out_generatorSettings, kodgen::MacroCodeGenUnitSettings& out_cguSettings)
{
	fs::path includeDirectory	= workingDirectory / "Include";
	fs::path generatedDirectory	= includeDirectory / "Generated";

	//Parse WorkingDir/...
	out_generatorSettings.addToParseDirectory(includeDirectory);

	//Ignore generated files...
	out_generatorSettings.addIgnoredDirectory(generatedDirectory);

	//Only parse .h files
	out_generatorSettings.addSupportedExtension(".h");

	//All generated files will be located in WorkingDir/Include/Generated
	out_cguSettings.setOutputDirectory(generatedDirectory);
	
	//Setup generated files name pattern
	out_cguSettings.setGeneratedHeaderFileNamePattern("##FILENAME##.h.h");
	out_cguSettings.setGeneratedSourceFileNamePattern("##FILENAME##.src.h");
	out_cguSettings.setClassFooterMacroPattern("##CLASSFULLNAME##_GENERATED");
	out_cguSettings.setHeaderFileFooterMacroPattern("File_##FILENAME##_GENERATED");
}

int main(int argc, char** argv)
{
	kodgen::DefaultLogger logger;

	if (argc <= 1)
	{
		logger.log("No working directory provided as first program argument", kodgen::ILogger::ELogSeverity::Error);
		return EXIT_FAILURE;
	}

	fs::path workingDirectory = argv[1];

	if (!fs::is_directory(workingDirectory))
	{
		logger.log("Provided working directory is not a directory or doesn't exist", kodgen::ILogger::ELogSeverity::Error);
		return EXIT_FAILURE;
	}

	logger.log("Working Directory: " + workingDirectory.string(), kodgen::ILogger::ELogSeverity::Info);

	//Setup File parser factory
	CppPropsParserFactory fileParserFactory;
	fileParserFactory.logger = &logger;

	//This is setuped that way for CI tools only
	//In reality, the compiler used by the user machine running the generator should be set.
	//It has nothing to see with the compiler used to compile the generator.
#if defined(__GNUC__)
	bool compilerSetSuccessfully = fileParserFactory.parsingSettings.setCompilerExeName("gcc");
#elif defined(__clang__)
	bool compilerSetSuccessfully = fileParserFactory.parsingSettings.setCompilerExeName("clang");
#elif defined(_MSC_VER)
	bool compilerSetSuccessfully = fileParserFactory.parsingSettings.setCompilerExeName("msvc");
#endif

	if (!compilerSetSuccessfully)
	{
		logger.log("Compiler could not be set because it is not supported on the current machine or vswhere could not be found (Windows|MSVC only).", kodgen::ILogger::ELogSeverity::Error);
		return EXIT_FAILURE;
	}

	//Setup settings
	kodgen::FileGeneratorSettings		fileGenSettings;
	kodgen::MacroCodeGenUnitSettings	cguSettings;

	initGenerationSettings(workingDirectory, fileGenSettings, cguSettings);

	//Setup code generation unit
	kodgen::CodeGenModuleGroup codeGenModuleGroup;

	GetSetCGM getSetCodeGenModule;
	codeGenModuleGroup.addModule(getSetCodeGenModule);

	kodgen::MacroCodeGenUnit codeGenUnit;
	codeGenUnit.codeGenModuleGroup = &codeGenModuleGroup;
	codeGenUnit.logger = &logger;
	codeGenUnit.setSettings(&cguSettings);

	//Setup generation settings
	kodgen::FileGenerator fileGenerator;
	fileGenerator.logger = &logger;
	fileGenerator.settings = &fileGenSettings;

	//Kick-off code generation
	kodgen::FileGenerationResult genResult = fileGenerator.generateFiles(fileParserFactory, codeGenUnit, true);

	if (genResult.completed)
	{
		logger.log("Generation completed successfully.");
	}
	else
	{
		logger.log("Invalid FileGenerator::outputDirectory", kodgen::ILogger::ELogSeverity::Error);
	}

	return EXIT_SUCCESS;
}