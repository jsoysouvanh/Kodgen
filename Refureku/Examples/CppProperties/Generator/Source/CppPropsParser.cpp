#include "CppPropsParser.h"

CppPropsParser::CppPropsParser() noexcept:
	FileParser()
{
	refureku::ParsingSettings& parsingSettings = getParsingSettings();

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

void CppPropsParser::preParse(fs::path const& parseFile) noexcept
{
	std::cout << "Start parsing: " << parseFile.string() << std::endl;
}

void CppPropsParser::postParse(fs::path const& parseFile, refureku::ParsingResult const& result) noexcept
{
	for (refureku::ParsingError const& parsingError : result.parsingErrors)
	{
		std::cout << parsingError << std::endl;
	}

	std::cout << "Found " << result.classes.size() << " classes and " << result.enums.size() << " enums." << std::endl;

	static_cast<void>(parseFile);
}