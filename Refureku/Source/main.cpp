#include <iostream>
#include <cassert>

#include "SafeFilesystem.h"

#include "FileGenerator.h"
#include "Parser.h"

#include "Properties/ComplexPropertyRule.h"
#include "Properties/PropertyRules.h"

#include "TestClass.h"

void propertyTests()
{
	refureku::PropertyRules propertyRules;

	assert(propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")));
	assert(!propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")));
	assert(propertyRules.removeSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")));
	assert(propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")));
	assert(propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp2")));

	assert(propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")));
	assert(!propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")));
	assert(propertyRules.removeComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")));
	assert(propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")));
	assert(propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp2", "BlueprintRead(Only|Write)")));

	assert(propertyRules.getSimpleProperty("RefurekuProp") != nullptr);
	assert(propertyRules.getComplexProperty("RefurekuProp") != nullptr);
	assert(propertyRules.getSimpleProperty("RefurekuProp2") != nullptr);
	assert(propertyRules.getComplexProperty("RefurekuProp2") != nullptr);
	assert(propertyRules.getSimpleProperty("RefurekuProp3") == nullptr);
	assert(propertyRules.getComplexProperty("RefurekuProp3") == nullptr);
}

int main()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	fs::path pathToFile		= includeDirPath / "TestClass.h";
	
	//	std::cout << includeDirPath.string() << std::endl;

	refureku::FileGenerator fg;
	refureku::Parser parser;

	//parser.parse(pathToFile);

	//fg.AddFile(fs::current_path());
	//fg.AddDirectory(fs::current_path());

	propertyTests();

	return EXIT_SUCCESS;
}