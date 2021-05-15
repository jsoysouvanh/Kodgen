#include "CppPropsParserFactory.h"

CppPropsParserFactory::CppPropsParserFactory() noexcept:
	kodgen::FileParserFactory<CppPropsParser>()
{
	//We abort parsing if we encounter a single error while parsing
	parsingSettings.shouldAbortParsingOnFirstError = true;

	//Each property will be separed by a ,
	parsingSettings.propertyParsingSettings.propertySeparator = ',';

	//Subproperties are surrounded by []
	parsingSettings.propertyParsingSettings.argumentEnclosers[0] = '[';
	parsingSettings.propertyParsingSettings.argumentEnclosers[1] = ']';

	//Each subproperty will be separed by a ,
	parsingSettings.propertyParsingSettings.argumentSeparator = ',';

	////Define the Get property, which can take
	parsingSettings.propertyParsingSettings.propertyRules.push_back(&_getPropertyRule);
	parsingSettings.propertyParsingSettings.propertyRules.push_back(&_setPropertyRule);

	//Define the macros to use for each entity type
	parsingSettings.propertyParsingSettings.namespaceMacroName	= "KGNamespace";
	parsingSettings.propertyParsingSettings.classMacroName		= "KGClass";
	parsingSettings.propertyParsingSettings.structMacroName		= "KGStruct";
	parsingSettings.propertyParsingSettings.fieldMacroName		= "KGVariable";
	parsingSettings.propertyParsingSettings.fieldMacroName		= "KGField";
	parsingSettings.propertyParsingSettings.functionMacroName	= "KGFunction";
	parsingSettings.propertyParsingSettings.methodMacroName		= "KGMethod";
	parsingSettings.propertyParsingSettings.enumMacroName		= "KGEnum";
	parsingSettings.propertyParsingSettings.enumValueMacroName	= "KGEnumVal";
}