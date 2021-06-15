#include "Kodgen/CodeGen/CodeGenUnitSettings.h"

#include "Kodgen/Misc/TomlUtility.h"
#include "Kodgen/Misc/ILogger.h"

using namespace kodgen;

bool CodeGenUnitSettings::loadSettingsValues(toml::value const& tomlData, ILogger* logger) noexcept
{
	if (tomlData.contains(tomlSectionName))
	{
		toml::value const& tomlGeneratorSettings = toml::find(tomlData, tomlSectionName);

		loadOutputDirectory(tomlGeneratorSettings, logger);
		
		return true;
	}
	else if (logger != nullptr)
	{
		logger->log("Could not find the [" + std::string(tomlSectionName) + "] section in the TOML file.", ILogger::ELogSeverity::Warning);
	}

	return false;
}

void CodeGenUnitSettings::loadOutputDirectory(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::string loadedOutputDirectory;

	if (TomlUtility::updateSetting(generationSettings, "outputDirectory", loadedOutputDirectory, logger))
	{
		bool success = setOutputDirectory(loadedOutputDirectory);

		if (logger != nullptr)
		{
			if (success)
			{
				logger->log("[TOML] Load output directory: " + getOutputDirectory().string());
			}
			else
			{
				logger->log("[TOML] Failed to load outputDirectory, file or invalid path: " + loadedOutputDirectory);
			}
		}
	}
}

fs::path const& CodeGenUnitSettings::getOutputDirectory() const noexcept
{
	return _outputDirectory;
}

bool CodeGenUnitSettings::setOutputDirectory(fs::path outputDirectory) noexcept
{
	if (!outputDirectory.empty())
	{
		outputDirectory.make_preferred();

		if (fs::exists(outputDirectory))
		{
			if (fs::is_directory(outputDirectory))
			{
				_outputDirectory = FilesystemHelpers::sanitizePath(outputDirectory);

				return true;
			}

			//the path points to a file
		}
		else //the provided path doesn't exist
		{
			//Make it absolute first
			std::error_code error;
			outputDirectory = fs::absolute(outputDirectory, error);

			if (!outputDirectory.empty())
			{
				_outputDirectory = outputDirectory;

				return true;
			}
		}
	}

	return false;
}