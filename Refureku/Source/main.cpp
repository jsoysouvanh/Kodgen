#include <iostream>

#include "SafeFilesystem.h"

#include "FileGenerator.h"
#include "Parser.h"

#include "TestClass.h"

int main()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	fs::path pathToFile		= includeDirPath / "TestClass.h";
	
	//	std::cout << includeDirPath.string() << std::endl;

	refureku::FileGenerator fg;
	refureku::Parser parser;

	parser.parse(pathToFile);

	fg.AddFile(fs::current_path());
	fg.AddDirectory(fs::current_path());

	return EXIT_SUCCESS;
}