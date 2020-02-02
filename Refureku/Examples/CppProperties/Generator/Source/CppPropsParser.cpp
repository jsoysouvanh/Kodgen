#include "CppPropsParser.h"

CppPropsParser::CppPropsParser() noexcept:
	Parser()
{
	//We abort parsing if we encounter a single error while parsing
	parsingSettings.shouldAbortParsingOnFirstError = true;

	//Ignore and remove space character before collecting properties
	parsingSettings.propertyParsingSettings.ignoredCharacters = {' '};

	//Each property will be separed by a ,
	parsingSettings.propertyParsingSettings.propertySeparator = ',';

	//Subproperties are surrounded by []
	parsingSettings.propertyParsingSettings.subPropertyEnclosers[0] = '[';
	parsingSettings.propertyParsingSettings.subPropertyEnclosers[1] = ']';

	//Each subproperty will be separed by a ,
	parsingSettings.propertyParsingSettings.subPropertySeparator = ',';

	refureku::PropertyRules& fieldPropertyRules = parsingSettings.propertyParsingSettings.fieldPropertyRules;

	//Define the Get property, which can take 
	fieldPropertyRules.addComplexPropertyRule("Get", "&|\\*|const|explicit");
	fieldPropertyRules.addComplexPropertyRule("Set", "explicit");
}