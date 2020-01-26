#include <iostream>
#include <cassert>

#include "Parsing/Parser.h"

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

	//EnumProp1, EnumProp2[EnumSubProp21, EnumSubProp22], EnumProp3
	refureku::PropertyRules& enumPropertyRules = parser.parsingSettings.propertyParsingSettings.enumPropertyRules;

	enumPropertyRules.addSimplePropertyRule("EnumProp1");
	enumPropertyRules.addSimplePropertyRule("EnumProp2");
	enumPropertyRules.addComplexPropertyRule("EnumProp2", "EnumSubProp2[1-9]");
	enumPropertyRules.addSimplePropertyRule("EnumProp3");

	//EnumValueProp1, EnumValueProp2[EnumValueSubProp21, EnumValueSubProp22], EnumValueProp3
	refureku::PropertyRules& enumValuePropertyRules = parser.parsingSettings.propertyParsingSettings.enumValuePropertyRules;

	enumValuePropertyRules.addSimplePropertyRule("EnumValueProp1");
	enumValuePropertyRules.addSimplePropertyRule("EnumValueProp2");
	enumValuePropertyRules.addComplexPropertyRule("EnumValueProp2", "EnumValueSubProp2[1-9]");
	enumValuePropertyRules.addSimplePropertyRule("EnumValueProp3");
}
//
//void parsingTests()
//{
//	fs::path includeDirPath	= fs::current_path().parent_path().parent_path().parent_path() / "Include";
//	fs::path pathToFile		= includeDirPath / "TestClass.h";
//
//	refureku::Parser parser;
//
//	setupParser(parser);
//
//	//Check result
//	refureku::ParsingResult parsingResult;
//	bool success = parser.parse(pathToFile, parsingResult);
//
//	if (success)
//	{
//		std::cout << "Parse success" << std::endl;
//
//		printClasses(parsingResult);
//	}
//	else
//	{
//		std::cout << "Parse ended with errors" << std::endl;
//
//		printErrors(parsingResult);
//	}
//}

int main()
{
	std::cout << fs::current_path().string() << std::endl;

	//TODO

	return EXIT_SUCCESS;
}