#include "Kodgen/Properties/PropertyParsingSettings.h"

#include "Kodgen/Misc/TomlUtility.h"
#include "Kodgen/Misc/ILogger.h"

using namespace kodgen;

bool PropertyParsingSettings::loadSettingsValues(toml::value const& tomlData, ILogger* logger) noexcept
{
	TomlUtility::updateSetting(tomlData, "propertySeparator", propertySeparator, logger);
	TomlUtility::updateSetting(tomlData, "subPropertySeparator", argumentSeparator, logger);
	TomlUtility::updateSetting(tomlData, "subPropertyStartEncloser", argumentEnclosers[0], logger);
	TomlUtility::updateSetting(tomlData, "subPropertyEndEncloser", argumentEnclosers[1], logger);

	TomlUtility::updateSetting(tomlData, "namespaceMacroName", namespaceMacroName, logger);
	TomlUtility::updateSetting(tomlData, "classMacroName", classMacroName, logger);
	TomlUtility::updateSetting(tomlData, "structMacroName", structMacroName, logger);
	TomlUtility::updateSetting(tomlData, "variableMacroName", variableMacroName, logger);
	TomlUtility::updateSetting(tomlData, "fieldMacroName", fieldMacroName, logger);
	TomlUtility::updateSetting(tomlData, "functionMacroName", functionMacroName, logger);
	TomlUtility::updateSetting(tomlData, "methodMacroName", methodMacroName, logger);
	TomlUtility::updateSetting(tomlData, "enumMacroName", enumMacroName, logger);
	TomlUtility::updateSetting(tomlData, "enumValueMacroName", enumValueMacroName, logger);

	return true;
}