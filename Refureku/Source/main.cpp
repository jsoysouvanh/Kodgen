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

void parsingTests()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	fs::path pathToFile		= includeDirPath / "TestClass.h";

	refureku::Parser parser;

	//Setup parser settings
	parser.parsingSettings.propertyParsingSettings.ignoredCharacters.insert(' ');	//Ignored white space
	parser.parsingSettings.propertyParsingSettings.subPropertySeparator = '/';

	assert(parser.retrieveParsingResult() == nullptr);

	bool success = parser.parse(pathToFile);

	//Check result
	refureku::ParsingResult const* result = parser.retrieveParsingResult();
	assert(result != nullptr);

	if (success)
	{
		std::cout << "Parse success" << std::endl;
	}
	else
	{
		std::cout << "Parse ended with errors" << std::endl;

		//Print errors
		for (refureku::ParsingError const& error : result->parsingErrors)
		{
			std::cout << "Error: " << "file: " << error.getFilename() << ", line: " << error.getLine() << ", column: " << error.getColumn() << std::endl;
		}
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