#include "Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h"

#include <algorithm>

#include "Kodgen/InfoStructures/StructClassInfo.h"
#include "Kodgen/Misc/TomlUtility.h"

using namespace kodgen;

bool MacroCodeGenUnitSettings::loadSettingsValues(toml::value const& tomlData, ILogger* logger) noexcept
{
	if (CodeGenUnitSettings::loadSettingsValues(tomlData, logger))
	{
		toml::value const& tomlMacroCGUSettings = toml::find(tomlData, tomlSectionName);

		loadFileNamePatterns(tomlMacroCGUSettings, logger);
		loadClassFooterMacroPattern(tomlMacroCGUSettings, logger);
		loadHeaderFileFooterMacroPattern(tomlMacroCGUSettings, logger);

		return true;
	}

	return false;
}

void MacroCodeGenUnitSettings::loadFileNamePatterns(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::string generatedFileNamePattern;

	//Load generated header file name
	if (TomlUtility::updateSetting(generationSettings, "generatedHeaderFileNamePattern", generatedFileNamePattern, logger))
	{
		setGeneratedHeaderFileNamePattern(generatedFileNamePattern);

		if (logger != nullptr)
		{
			logger->log("[TOML] Load generated header file name pattern: " + _generatedHeaderFileNamePattern);
		}
	}

	//Load generated source file name
	if (TomlUtility::updateSetting(generationSettings, "generatedSourceFileNamePattern", generatedFileNamePattern, logger))
	{
		setGeneratedSourceFileNamePattern(generatedFileNamePattern);

		if (logger != nullptr)
		{
			logger->log("[TOML] Load generated source file name pattern: " + _generatedSourceFileNamePattern);
		}
	}
}

void MacroCodeGenUnitSettings::loadClassFooterMacroPattern(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::string classFooterMacroPattern;

	//Load class footer macro pattern
	if (TomlUtility::updateSetting(generationSettings, "classFooterMacroPattern", classFooterMacroPattern, logger))
	{
		setClassFooterMacroPattern(classFooterMacroPattern);

		if (logger != nullptr)
		{
			logger->log("[TOML] Load class footer macro pattern: " + _generatedHeaderFileNamePattern);
		}
	}
}

void MacroCodeGenUnitSettings::loadHeaderFileFooterMacroPattern(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::string headerFileFooterMacroPattern;

	//Load class footer macro pattern
	if (TomlUtility::updateSetting(generationSettings, "headerFileFooterMacroPattern", headerFileFooterMacroPattern, logger))
	{
		setHeaderFileFooterMacroPattern(headerFileFooterMacroPattern);

		if (logger != nullptr)
		{
			logger->log("[TOML] Load header file footer macro pattern: " + _generatedHeaderFileNamePattern);
		}
	}
}

void MacroCodeGenUnitSettings::setGeneratedHeaderFileNamePattern(std::string const& generatedHeaderFileNamePattern) noexcept
{
	_generatedHeaderFileNamePattern = generatedHeaderFileNamePattern;
}

void MacroCodeGenUnitSettings::setGeneratedSourceFileNamePattern(std::string const& generatedSourceFileNamePattern) noexcept
{
	_generatedSourceFileNamePattern = generatedSourceFileNamePattern;
}

void MacroCodeGenUnitSettings::setClassFooterMacroPattern(std::string const& classFooterMacroPattern) noexcept
{
	_classFooterMacroPattern = classFooterMacroPattern;
}

void MacroCodeGenUnitSettings::setHeaderFileFooterMacroPattern(std::string const& headerFileFooterMacroPattern) noexcept
{
	_headerFileFooterMacroPattern = headerFileFooterMacroPattern;
}

std::string const& MacroCodeGenUnitSettings::getGeneratedHeaderFileNamePattern() const noexcept
{
	return _generatedHeaderFileNamePattern;
}

fs::path MacroCodeGenUnitSettings::getGeneratedHeaderFileName(fs::path const& targetFile) const noexcept
{
	std::string	filename = _generatedHeaderFileNamePattern;
	
	//Replace all occurences of ##FILENAME## by the targetFile name (without its extension)
	replaceTags(filename, filenameTag, targetFile.filename().stem().string());

	return filename;
}

std::string const&	MacroCodeGenUnitSettings::getGeneratedSourceFileNamePattern() const noexcept
{
	return _generatedSourceFileNamePattern;
}

fs::path MacroCodeGenUnitSettings::getGeneratedSourceFileName(fs::path const& targetFile) const noexcept
{
	std::string	filename = _generatedSourceFileNamePattern;

	//Replace all occurences of ##FILENAME## by the targetFile name (without its extension)
	replaceTags(filename, filenameTag, targetFile.filename().stem().string());

	return filename;
}

std::string const& MacroCodeGenUnitSettings::getClassFooterMacroPattern() const noexcept
{
	return _classFooterMacroPattern;
}

std::string MacroCodeGenUnitSettings::getClassFooterMacro(StructClassInfo const& structClassInfo) const noexcept
{
	std::string	classFooterMacroName	= _classFooterMacroPattern;
	std::string classFullName			= structClassInfo.getFullName();

	//Replace full name :: into _ so that it makes a valid macro
	replaceTags(classFullName, "::", "_");
	replaceTags(classFooterMacroName, classNameTag, structClassInfo.name);
	replaceTags(classFooterMacroName, classFullNameTag, classFullName);

	return classFooterMacroName;
}

std::string const& MacroCodeGenUnitSettings::getHeaderFileFooterMacroPattern() const noexcept
{
	return _headerFileFooterMacroPattern;
}

std::string	MacroCodeGenUnitSettings::getHeaderFileFooterMacro(fs::path const& targetFile) const noexcept
{
	std::string headerFileFooterMacroName = _headerFileFooterMacroPattern;

	replaceTags(headerFileFooterMacroName, filenameTag, targetFile.filename().stem().string());

	return headerFileFooterMacroName;
}

void MacroCodeGenUnitSettings::replaceTags(std::string& inout_string, std::string_view const& tag, std::string const& replacement) noexcept
{
	size_t index = inout_string.find(tag, 0u);

	while (index != std::string::npos)
	{
		inout_string.replace(index, tag.size(), replacement);

		index = inout_string.find(tag, 0u);
	}
}