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

	assert(propertyRules.addSimplePropertyRule("RefurekuProp"));
	assert(!propertyRules.addSimplePropertyRule("RefurekuProp"));
	assert(propertyRules.removeSimplePropertyRule("RefurekuProp"));
	assert(propertyRules.addSimplePropertyRule("RefurekuProp"));
	assert(propertyRules.addSimplePropertyRule("RefurekuProp2"));

	assert(propertyRules.addComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)"));
	assert(!propertyRules.addComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)"));
	assert(propertyRules.removeComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)"));
	assert(propertyRules.addComplexPropertyRule("RefurekuProp", "BlueprintRead(Only|Write)"));
	assert(propertyRules.addComplexPropertyRule("RefurekuProp2", "BlueprintRead(Only|Write)"));

	assert(propertyRules.getSimplePropertyRule("RefurekuProp") != nullptr);
	assert(propertyRules.getComplexPropertyRule("RefurekuProp") != nullptr);
	assert(propertyRules.getSimplePropertyRule("RefurekuProp2") != nullptr);
	assert(propertyRules.getComplexPropertyRule("RefurekuProp2") != nullptr);
	assert(propertyRules.getSimplePropertyRule("RefurekuProp3") == nullptr);
	assert(propertyRules.getComplexPropertyRule("RefurekuProp3") == nullptr);
}

void printClasses(refureku::ParsingResult const* result)
{
	//Print classes
	for (refureku::ClassInfo const& c : result->classes)
	{
		std::cout << "Class: " << c.name << std::endl;
		for (refureku::SimpleProperty const& sp : c.properties.simpleProperties)
		{
			std::cout << sp.name << std::endl;
		}
		for (refureku::ComplexProperty const& cp : c.properties.complexProperties)
		{
			std::cout << cp.name;

			for (std::string const& subprop : cp.subProperties)
			{
				std::cout << " " << subprop;
			}

			std::cout << std::endl;
		}
	}
}

void printErrors(refureku::ParsingResult const* result)
{
	for (refureku::ParsingError const& error : result->parsingErrors)
	{
		std::cout << "Error: " << static_cast<refureku::uint16>(error.getErrorValue()) << ", file: " << error.getFilename() << ", line: " << error.getLine() << ", column: " << error.getColumn() << std::endl;
	}
}

void parsingTests()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	fs::path pathToFile		= includeDirPath / "TestClass.h";

	refureku::Parser parser;

	//Setup parser settings
	parser.parsingSettings.shouldAbortParsingOnFirstError = false;

	parser.parsingSettings.propertyParsingSettings.ignoredCharacters.insert(' ');	//Ignore white space
	parser.parsingSettings.propertyParsingSettings.subPropertySeparator = '/';

	//Prop1, Prop2[SubProp21/ SubProp22], Prop3
	parser.parsingSettings.propertyParsingSettings.classPropertyRules.addSimplePropertyRule("Prop1");
	parser.parsingSettings.propertyParsingSettings.classPropertyRules.addComplexPropertyRule("Prop2", "SubProp21|SubProp22");
	parser.parsingSettings.propertyParsingSettings.classPropertyRules.addSimplePropertyRule("Prop3");

	assert(parser.retrieveParsingResult() == nullptr);

	bool success = parser.parse(pathToFile);

	//Check result
	refureku::ParsingResult const* result = parser.retrieveParsingResult();
	assert(result != nullptr);

	if (success)
	{
		std::cout << "Parse success" << std::endl;

		printClasses(result);
	}
	else
	{
		std::cout << "Parse ended with errors" << std::endl;

		printErrors(result);
	}
}

void randomTests()
{
	std::string str = "abcdef";

	std::cout << str.substr(0, 5) << std::endl;
}

int main()
{
	//propertyTests();

	parsingTests();

	//randomTests();

	return EXIT_SUCCESS;
}