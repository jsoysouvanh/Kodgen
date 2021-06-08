#include <iostream>

#include <Kodgen/Misc/Filesystem.h>
#include <Kodgen/Misc/DefaultLogger.h>
#include <Kodgen/CodeGen/FileGenerator.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnit.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h>
#include <Kodgen/CodeGen/FileGeneratorSettings.h>

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

bool initParsingSettings(kodgen::ParsingSettings& parsingSettings)
{
	//We abort parsing if we encounter a single error while parsing
	parsingSettings.shouldAbortParsingOnFirstError = true;

	//Each property will be separed by a ,
	parsingSettings.propertyParsingSettings.propertySeparator = ',';

	//Subproperties are surrounded by []
	parsingSettings.propertyParsingSettings.argumentEnclosers[0] = '[';
	parsingSettings.propertyParsingSettings.argumentEnclosers[1] = ']';

	//Each subproperty will be separed by a ,
	parsingSettings.propertyParsingSettings.argumentSeparator = ',';

	//Define the macros to use for each entity type
	parsingSettings.propertyParsingSettings.namespaceMacroName	= "KGNamespace";
	parsingSettings.propertyParsingSettings.classMacroName		= "KGClass";
	parsingSettings.propertyParsingSettings.structMacroName		= "KGStruct";
	parsingSettings.propertyParsingSettings.fieldMacroName		= "KGVariable";
	parsingSettings.propertyParsingSettings.fieldMacroName		= "KGField";
	parsingSettings.propertyParsingSettings.functionMacroName	= "KGFunction";
	parsingSettings.propertyParsingSettings.methodMacroName		= "KGMethod";
	parsingSettings.propertyParsingSettings.enumMacroName		= "KGEnum";
	parsingSettings.propertyParsingSettings.enumValueMacroName	= "KGEnumVal";

	//This is setuped that way for CI tools only
	//In reality, the compiler used by the user machine running the generator should be set.
	//It has nothing to see with the compiler used to compile the generator.
#if defined(__GNUC__)
	bool compilerSetSuccessfully = parsingSettings.setCompilerExeName("gcc");
#elif defined(__clang__)
	bool compilerSetSuccessfully = parsingSettings.setCompilerExeName("clang");
#elif defined(_MSC_VER)
	bool compilerSetSuccessfully = parsingSettings.setCompilerExeName("msvc");
#endif

	return compilerSetSuccessfully;
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

	//Setup parsing settings
	kodgen::ParsingSettings parsingSettings;

	if (!initParsingSettings(parsingSettings))
	{
		logger.log("Compiler could not be set because it is not supported on the current machine or vswhere could not be found (Windows|MSVC only).", kodgen::ILogger::ELogSeverity::Error);
		return EXIT_FAILURE;
	}

	//Setup FileParser
	kodgen::FileParser fileParser;
	fileParser.logger = &logger;
	fileParser.parsingSettings = &parsingSettings;

	//Setup settings
	kodgen::FileGeneratorSettings		fileGenSettings;
	kodgen::MacroCodeGenUnitSettings	cguSettings;

	initGenerationSettings(workingDirectory, fileGenSettings, cguSettings);

	//Setup code generation unit
	kodgen::MacroCodeGenUnit codeGenUnit;
	codeGenUnit.logger = &logger;
	codeGenUnit.setSettings(&cguSettings);

	GetSetCGM getSetCodeGenModule;
	codeGenUnit.addModule(getSetCodeGenModule);

	//Setup generation settings
	kodgen::FileGenerator fileGenerator;
	fileGenerator.logger = &logger;
	fileGenerator.settings = &fileGenSettings;

	//Kick-off code generation
	kodgen::FileGenerationResult genResult = fileGenerator.generateFiles(fileParser, codeGenUnit, true);

	if (genResult.completed)
	{
		logger.log("Generation completed successfully.");
	}

	return EXIT_SUCCESS;
}