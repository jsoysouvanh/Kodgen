#include "Kodgen/CodeGen/FileGenerationSettings.h"

#include "Kodgen/Misc/TomlUtility.h"

using namespace kodgen;

bool FileGenerationSettings::loadSettingsValues(toml::value const& tomlData, ILogger* logger) noexcept
{
	if (tomlData.contains(_tomlSettingsSectionName))
	{
		toml::value const& tomlGeneratorSettings = toml::find(tomlData, _tomlSettingsSectionName);
		
		loadGeneratedFilesExtension(tomlGeneratorSettings, logger);
		loadEntityMacrosFilename(tomlGeneratorSettings, logger);
		loadSupportedExtensions(tomlGeneratorSettings, logger);
		loadOutputDirectory(tomlGeneratorSettings, logger);
		loadToParseFiles(tomlGeneratorSettings, logger);
		loadToParseDirectories(tomlGeneratorSettings, logger);
		loadIgnoredFiles(tomlGeneratorSettings, logger);
		loadIgnoredDirectories(tomlGeneratorSettings, logger);

		return true;
	}
	else if (logger != nullptr)
	{
		logger->log("Could not find the [" + std::string(_tomlSettingsSectionName) + "] section in the TOML file.", ILogger::ELogSeverity::Warning);
	}

	return false;
}

bool FileGenerationSettings::setOutputDirectory(fs::path outputDirectory) noexcept
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

bool FileGenerationSettings::addToParseFile(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && !fs::is_directory(sanitizedPath))
	{
		return _toParseFiles.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

bool FileGenerationSettings::addToParseDirectory(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && fs::is_directory(sanitizedPath))
	{
		return _toParseDirectories.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

bool FileGenerationSettings::addIgnoredFile(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && !fs::is_directory(sanitizedPath))
	{
		return _ignoredFiles.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

bool FileGenerationSettings::addIgnoredDirectory(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && fs::is_directory(sanitizedPath))
	{
		return _ignoredDirectories.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

void FileGenerationSettings::removeToParseFile(fs::path const& path) noexcept
{
	_toParseFiles.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGenerationSettings::removeToParseDirectory(fs::path const& path) noexcept
{
	_toParseDirectories.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGenerationSettings::removeIgnoredFile(fs::path const& path) noexcept
{
	_ignoredFiles.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGenerationSettings::removeIgnoredDirectory(fs::path const& path) noexcept
{
	_ignoredDirectories.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGenerationSettings::clearToParseFiles() noexcept
{
	_toParseFiles.clear();
}

void FileGenerationSettings::clearToParseDirectories() noexcept
{
	_toParseDirectories.clear();
}

void FileGenerationSettings::clearIgnoredFiles() noexcept
{
	_ignoredFiles.clear();
}

void FileGenerationSettings::clearIgnoredDirectories() noexcept
{
	_ignoredDirectories.clear();
}

void FileGenerationSettings::loadGeneratedFilesExtension(toml::value const& generationSettings, ILogger* logger) noexcept
{
	if (TomlUtility::updateSetting(generationSettings, "generatedFilesExtension", generatedFilesExtension, logger) && logger != nullptr)
	{
		logger->log("[TOML] Load generatedFilesExtension: " + generatedFilesExtension);
	}
}

void FileGenerationSettings::loadEntityMacrosFilename(toml::value const& generationSettings, ILogger* logger) noexcept
{
	if (TomlUtility::updateSetting(generationSettings, "entityMacrosFilename", entityMacrosFilename, logger) && logger != nullptr)
	{
		logger->log("[TOML] Load entityMacrosFilename: " + entityMacrosFilename);
	}
}

void FileGenerationSettings::loadSupportedExtensions(toml::value const& generationSettings, ILogger* logger) noexcept
{
	//Clear supported extensions before loading
	supportedExtensions.clear();

	std::unordered_set<std::string> loadedExtensions;
	if (TomlUtility::updateSetting(generationSettings, "supportedExtensions", loadedExtensions, logger) && logger != nullptr)
	{
		for (std::string const& extension : loadedExtensions)
		{
			//TODO: might add extension validity check later
			supportedExtensions.emplace(extension);

			if (logger != nullptr)
			{
				logger->log("[TOML] Load new supported extension: " + extension);
			}
		}
	}
}

void FileGenerationSettings::loadOutputDirectory(toml::value const& generationSettings, ILogger* logger) noexcept
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

void FileGenerationSettings::loadToParseFiles(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> toParseFiles;

	clearToParseFiles();

	if (TomlUtility::updateSetting(generationSettings, "toParseFiles", toParseFiles, logger))
	{
		bool success;

		for (fs::path const& path : toParseFiles)
		{
			success = addToParseFile(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new file to parse: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add toParseFile as it doesn't exist, is not a file or is already part of the list of files to parse: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}

void FileGenerationSettings::loadToParseDirectories(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> toParseDirectories;

	clearToParseDirectories();

	if (TomlUtility::updateSetting(generationSettings, "toParseDirectories", toParseDirectories, logger))
	{
		bool success;

		for (fs::path const& path : toParseDirectories)
		{
			success = addToParseDirectory(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new directory to parse: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add toParseDirectory as it doesn't exist, is not a directory or is already part of the list of directories to parse: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}

void FileGenerationSettings::loadIgnoredFiles(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> ignoredFiles;

	clearIgnoredFiles();

	if (TomlUtility::updateSetting(generationSettings, "ignoredFiles", ignoredFiles, logger))
	{
		bool success;

		for (fs::path const& path : ignoredFiles)
		{
			success = addIgnoredFile(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new ignored file: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add ignoredFile as it doesn't exist, is not a file or is already part of the list of ignored files: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}

void FileGenerationSettings::loadIgnoredDirectories(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> ignoredDirectories;

	clearIgnoredDirectories();

	if (TomlUtility::updateSetting(generationSettings, "ignoredDirectories", ignoredDirectories, logger))
	{
		bool success;

		for (fs::path const& path : ignoredDirectories)
		{
			success = addIgnoredDirectory(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new ignored directory: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add ignoredDirectory as it doesn't exist, is not a directory or is already part of the list of ignored directories: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}