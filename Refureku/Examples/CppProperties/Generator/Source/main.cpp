#include <iostream>

#include <Misc/Filesystem.h>
#include <CodeGen/FileGenerator.h>

#include "CppPropsParser.h"
#include "CppPropsCodeTemplate.h"

int main(int argc, char** argv)
{
	int result = EXIT_SUCCESS;

	if (argc > 1)
	{
		fs::path workingDirectory = argv[1];

		if (fs::is_directory(workingDirectory))
		{
			std::cout << "WORKING DIRECTORY IS: " << workingDirectory.string() << std::endl;

			fs::path includeDirectory = workingDirectory / "Include";

			CppPropsParser parser;
			refureku::FileGenerator fileGenerator;

			//Parse WorkingDir/Include/SomeClass.h
			fileGenerator.addFile(includeDirectory / "SomeClass.h");
			fileGenerator.addFile(includeDirectory / "SomeOtherClass.h");

			//All generated files will be located in WorkingDir/Include/Generated
			fileGenerator.outputDirectory = includeDirectory / "Generated";

			//Generated files will use .myCustomExtension.h extension
			fileGenerator.generatedFilesExtension = ".myCustomExtension.h";

			//Bind the PropertyCodeTemplate name to the CppPropsCodeTemplate class
			fileGenerator.addGeneratedCodeTemplate("PropertyCodeTemplate", new CppPropsCodeTemplate());

			/**
			*	Can specify code template to use by using the MyCustomCodeTemplatePropertyName main property
			*
			*	For example:
			*		RkrfField(MyCustomCodeTemplatePropertyName[PropertyCodeTemplate])
			*		int i;
			*/
			fileGenerator.codeTemplateMainComplexPropertyName = "MyCustomCodeTemplatePropertyName";

			/**
			*	Set a default class template so that we don't have to specify it manually
			*
			*	Now we can simply write:
			*		RkrfField()
			*		int i;
			*/
			fileGenerator.setDefaultClassTemplate("PropertyCodeTemplate");

			refureku::FileGenerationResult genResult = fileGenerator.generateFiles(parser, true);

			if (genResult.completed)
			{
				for (refureku::ParsingError parsingError : genResult.parsingErrors)
				{
					std::cerr << parsingError << std::endl;
				}

				for (refureku::FileGenerationError fileGenError : genResult.fileGenerationErrors)
				{
					std::cerr << fileGenError << std::endl;
				}
			}
			else
			{
				std::cerr << "Invalid FileGenerator::outputDirectory" << std::endl;
			}
		}
		else
		{
			std::cerr << "Provided working directory is not a directory or doesn't exist" << std::endl;
			result = EXIT_FAILURE;
		}
	}
	else
	{
		std::cerr << "No working directory provided as first program argument" << std::endl;
		result = EXIT_FAILURE;
	}

	return result;
}