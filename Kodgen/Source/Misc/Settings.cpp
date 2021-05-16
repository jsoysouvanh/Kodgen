#include "Kodgen/Misc/Settings.h"

#include "Kodgen/Misc/TomlUtility.h"

using namespace kodgen;

bool Settings::loadSettings(fs::path const& pathToSettingsFile, ILogger* logger) noexcept
{
	try
	{
		return loadSettingsValues(toml::parse(pathToSettingsFile.string()), logger);
	}
	catch (std::runtime_error const&)
	{
		//Failed to open the file
		if (logger != nullptr)
		{
			logger->log("Failed to load file generation settings at " + pathToSettingsFile.string(), ILogger::ELogSeverity::Error);
		}
	}
	catch (toml::syntax_error const& e)
	{
		if (logger != nullptr)
		{
			logger->log("Syntax error in settings file.\n" + std::string(e.what()), ILogger::ELogSeverity::Error);
		}
	}

	return false;
}