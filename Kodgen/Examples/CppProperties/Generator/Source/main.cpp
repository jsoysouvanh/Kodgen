#include <iostream>

#include <Kodgen/Misc/Filesystem.h>
#include <Kodgen/Misc/DefaultLogger.h>
#include <Kodgen/CodeGen/FileGenerator.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnit.h>
#include <Kodgen/CodeGen/CodeGenModuleGroup.h>

#include "CppPropsParserFactory.h"
#include "GetSetCGM.h"

void initGenerationSettings(fs::path const& workingDirectory, kodgen::FileGenerator::Settings& out_settings)
{
	fs::path includeDirectory	= workingDirectory / "Include";
	fs::path generatedDirectory	= includeDirectory / "Generated";

	//Parse WorkingDir/...
	out_settings.addToParseDirectory(includeDirectory);

	//Ignore generated files...
	out_settings.addIgnoredDirectory(generatedDirectory);

	//Only parse .h files
	out_settings.supportedExtensions.emplace(".h");

	//All generated files will be located in WorkingDir/Include/Generated
	out_settings.setOutputDirectory(generatedDirectory);

	//Generated files will use .myCustomExtension.h extension
	out_settings.generatedFilesExtension = ".myCustomExtension.h";
}

int main(int argc, char** argv)
{
	kodgen::DefaultLogger	logger;
	int						result = EXIT_SUCCESS;

	if (argc > 1)
	{
		fs::path workingDirectory = argv[1];

		if (fs::is_directory(workingDirectory))
		{
			logger.log("Working Directory: " + workingDirectory.string(), kodgen::ILogger::ELogSeverity::Info);

			//Setup File parser factory
			CppPropsParserFactory fileParserFactory;
			fileParserFactory.logger = &logger;
			fileParserFactory.parsingSettings.setCompilerExeName("clang++");

			//Setup code generation unit
			kodgen::CodeGenModuleGroup codeGenModuleGroup;

			GetSetCGM getSetCodeGenModule;
			codeGenModuleGroup.addModule(getSetCodeGenModule);

			kodgen::MacroCodeGenUnit codeGenUnit;
			codeGenUnit.codeGenModuleGroup = &codeGenModuleGroup;

			//TODO: 

			//Setup generation settings
			kodgen::FileGenerator::Settings	fileGenSettings;
			initGenerationSettings(workingDirectory, fileGenSettings);

			kodgen::FileGenerator fileGenerator;
			fileGenerator.logger = &logger;
			fileGenerator.settings = &fileGenSettings;

			//Bind the PropertyCodeTemplate name to the CppPropsCodeTemplate class
			//CppPropsCodeTemplate propsCodeTemplate;
			//fileGenerator.addGeneratedCodeTemplate("PropertyCodeTemplate", &propsCodeTemplate);

			///**
			//*	Set a default class template so that we don't have to specify it manually
			//*
			//*	Now we can simply write:
			//*		class KGClass() MyClass {};
			//*/
			//fileGenerator.setDefaultGeneratedCodeTemplate(kodgen::EEntityType::Class, "PropertyCodeTemplate");

			kodgen::FileGenerationResult genResult = fileGenerator.generateFiles(fileParserFactory, codeGenUnit, true);

			if (genResult.completed)
			{
				for (kodgen::ParsingError parsingError : genResult.parsingErrors)
				{
					logger.log(parsingError.toString(), kodgen::ILogger::ELogSeverity::Error);
				}

				for (kodgen::FileGenerationError fileGenError : genResult.fileGenerationErrors)
				{
					logger.log(fileGenError.toString(), kodgen::ILogger::ELogSeverity::Error);
				}
			}
			else
			{
				logger.log("Invalid FileGenerator::outputDirectory", kodgen::ILogger::ELogSeverity::Error);
			}
		}
		else
		{
			logger.log("Provided working directory is not a directory or doesn't exist", kodgen::ILogger::ELogSeverity::Error);
			result = EXIT_FAILURE;
		}
	}
	else
	{
		logger.log("No working directory provided as first program argument", kodgen::ILogger::ELogSeverity::Error);
		result = EXIT_FAILURE;
	}

	return result;
}