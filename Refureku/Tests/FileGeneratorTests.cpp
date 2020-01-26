#include <iostream>
#include <cassert>

//void fileGeneratorTests()
//{
//	//Setup parser
//	refureku::Parser parser;
//
//	setupParser(parser);
//
//	//Setup FileGenerator
//	refureku::FileGenerator fg;
//
//	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
//	fs::path pathToFile		= includeDirPath / "TestClass.h";
//
//	fg.pathToGeneratedFilesFolder = includeDirPath / "Generated";
//
//	fg.addGeneratedCodeTemplate("TestTemplate", new refureku::TestGeneratedCodeTemplate());
//	fg.setDefaultClassTemplate("TestTemplate");
//
//	//Add files/directories to parse
//	fg.addFile(pathToFile);
//
//	refureku::FileGenerationResult fgr = fg.generateFiles(parser, true);
//
//	std::cout << "File generation: " << ((fgr.completed) ? "Success" : "Failure") << std::endl;
//
//	for (refureku::ParsingError& pe : fgr.parsingErrors)
//	{
//		std::cout << pe << std::endl;
//	}
//
//	for (refureku::FileGenerationError& fge : fgr.fileGenerationErrors)
//	{
//		std::cout << fge << std::endl;
//	}
//}

int main()
{
	return EXIT_SUCCESS;
}