#include <iostream>

#include "SafeFilesystem.h"

#include "FileGenerator.h"
#include "Parser.h"

int main()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	//	fs::path pathToFile		= includeDirPath / "TestClass.h";
	//
	//	std::cout << includeDirPath.string() << std::endl;

	refureku::FileGenerator fg;
	refureku::Parser parser;

	fg.AddFile(fs::current_path());
	fg.AddDirectory(fs::current_path());

	return EXIT_SUCCESS;
}