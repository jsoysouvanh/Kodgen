#include <iostream>

#include "SafeFilesystem.h"

#include "FileGenerator.h"
#include "Parser.h"

#include "Properties/ComplexPropertyRule.h"
#include "Properties/PropertyRules.h"

#include "TestClass.h"

void regexTests()
{
	refureku::PropertyRules propertyRules;

	std::cout << std::boolalpha << propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")) << " : Should be true" << std::endl;
	std::cout << std::boolalpha << propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")) << " : Should be false" << std::endl;
	std::cout << std::boolalpha << propertyRules.removeSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")) << " : Should be true" << std::endl;
	std::cout << std::boolalpha << propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp")) << " : Should be true" << std::endl;
	std::cout << std::boolalpha << propertyRules.addSimpleProperty(refureku::SimplePropertyRule("RefurekuProp2")) << " : Should be true" << std::endl;

	std::cout << std::boolalpha << propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")) << " : Should be true" << std::endl;
	std::cout << std::boolalpha << propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")) << " : Should be false" << std::endl;
	std::cout << std::boolalpha << propertyRules.removeComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")) << " : Should be true" << std::endl;
	std::cout << std::boolalpha << propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)")) << " : Should be true" << std::endl;
	std::cout << std::boolalpha << propertyRules.addComplexProperty(refureku::ComplexPropertyRule("RefurekuProp2", "BlueprintRead(Only|Write)")) << " : Should be true" << std::endl;
}

int main()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	fs::path pathToFile		= includeDirPath / "TestClass.h";
	
	//	std::cout << includeDirPath.string() << std::endl;

	refureku::FileGenerator fg;
	refureku::Parser parser;

	//TODO PropertyParser, PropertyRules, SimpleProperty, ArgumentsProperty
	//ArgumentsProperty -> list of authorized arguments, can the user add its own arguments, might add later constraints on user arguments (check validity with regex)

	//parser.parse(pathToFile);

	//fg.AddFile(fs::current_path());
	//fg.AddDirectory(fs::current_path());

	regexTests();

	return EXIT_SUCCESS;
}