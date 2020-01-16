#include <iostream>
#include <cassert>

#include "CodeGen/FileGenerator.h"
#include "CodeGen/TestGeneratedCodeTemplate.h"
#include "Parsing/Parser.h"
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

void printClasses(refureku::ParsingResult const& result)
{
	//Print classes
	for (refureku::ClassInfo const& c : result.classes)
	{
		std::cout << c << std::endl;
	}
}

void printErrors(refureku::ParsingResult const& result)
{
	for (refureku::ParsingError const& error : result.parsingErrors)
	{
		std::cout << error << std::endl;
	}
}

void setupParser(refureku::Parser& parser)
{
	//Setup parser settings
	parser.parsingSettings.shouldAbortParsingOnFirstError = false;

	parser.parsingSettings.propertyParsingSettings.ignoredCharacters.insert(' ');	//Ignore white space
	parser.parsingSettings.propertyParsingSettings.subPropertySeparator = ',';

	//ClassProp1, ClassProp2[ClassSubProp21, ClassSubProp22], ClassProp3
	refureku::PropertyRules& classPropertyRules = parser.parsingSettings.propertyParsingSettings.classPropertyRules;

	classPropertyRules.addSimplePropertyRule("ClassProp1");
	classPropertyRules.addSimplePropertyRule("ClassProp2");
	classPropertyRules.addComplexPropertyRule("ClassProp2", "ClassSubProp2[1-9]");
	classPropertyRules.addSimplePropertyRule("ClassProp3");

	//MethodProp1, MethodProp2[MethodSubProp21, MethodSubProp22], MethodProp3
	refureku::PropertyRules& methodPropertyRules = parser.parsingSettings.propertyParsingSettings.methodPropertyRules;

	methodPropertyRules.addSimplePropertyRule("MethodProp1");
	methodPropertyRules.addSimplePropertyRule("MethodProp2");
	methodPropertyRules.addComplexPropertyRule("MethodProp2", "MethodSubProp2[1-9]");
	methodPropertyRules.addSimplePropertyRule("MethodProp3");

	//FieldProp1, FieldProp2[FieldSubProp21, FieldSubProp22], FieldProp3
	refureku::PropertyRules& fieldPropertyRules = parser.parsingSettings.propertyParsingSettings.fieldPropertyRules;

	fieldPropertyRules.addSimplePropertyRule("FieldProp1");
	fieldPropertyRules.addSimplePropertyRule("FieldProp2");
	fieldPropertyRules.addComplexPropertyRule("FieldProp2", "FieldSubProp2[1-9]");
	fieldPropertyRules.addSimplePropertyRule("FieldProp3");
}

void parsingTests()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	fs::path pathToFile		= includeDirPath / "TestClass.h";

	refureku::Parser parser;

	setupParser(parser);

	//Check result
	refureku::ParsingResult parsingResult;
	bool success = parser.parse(pathToFile, parsingResult);

	if (success)
	{
		std::cout << "Parse success" << std::endl;

		printClasses(parsingResult);
	}
	else
	{
		std::cout << "Parse ended with errors" << std::endl;

		printErrors(parsingResult);
	}
}

void fileGeneratorTests()
{
	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
	fs::path pathToFile		= includeDirPath / "TestClass.h";

	refureku::Parser parser;

	setupParser(parser);

	refureku::FileGenerator fg;

	fg.addGeneratedCodeTemplate("TestTemplate", new refureku::TestGeneratedCodeTemplate());
	fg.setDefaultClassTemplate("TestTemplate");

	fg.addFile(pathToFile);

	fg.generateFiles(parser, true);
}

int main()
{
	//propertyTests();

	//parsingTests();

	fileGeneratorTests();

	return EXIT_SUCCESS;
}