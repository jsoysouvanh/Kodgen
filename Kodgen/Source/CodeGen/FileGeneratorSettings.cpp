#include "Kodgen/CodeGen/FileGeneratorSettings.h"

#include "Kodgen/Misc/TomlUtility.h"
#include "Kodgen/Misc/ILogger.h"

using namespace kodgen;

bool FileGeneratorSettings::loadSettingsValues(toml::value const& tomlData, ILogger* logger) noexcept
{
	if (tomlData.contains(_tomlSectionName))
	{
		toml::value const& tomlGeneratorSettings = toml::find(tomlData, _tomlSectionName);
		
		loadSupportedExtensions(tomlGeneratorSettings, logger);
		loadToParseFiles(tomlGeneratorSettings, logger);
		loadToParseDirectories(tomlGeneratorSettings, logger);
		loadIgnoredFiles(tomlGeneratorSettings, logger);
		loadIgnoredDirectories(tomlGeneratorSettings, logger);

		return true;
	}
	else if (logger != nullptr)
	{
		logger->log("Could not find the [" + std::string(_tomlSectionName) + "] section in the TOML file.", ILogger::ELogSeverity::Warning);
	}

	return false;
}

bool FileGeneratorSettings::addToParseFile(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && !fs::is_directory(sanitizedPath))
	{
		return _toParseFiles.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

bool FileGeneratorSettings::addToParseDirectory(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && fs::is_directory(sanitizedPath))
	{
		return _toParseDirectories.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

bool FileGeneratorSettings::addIgnoredFile(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && !fs::is_directory(sanitizedPath))
	{
		return _ignoredFiles.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

bool FileGeneratorSettings::addIgnoredDirectory(fs::path const& path) noexcept
{
	fs::path sanitizedPath = FilesystemHelpers::sanitizePath(path);

	if (!sanitizedPath.empty() && fs::is_directory(sanitizedPath))
	{
		return _ignoredDirectories.emplace(std::move(sanitizedPath)).second;
	}

	return false;
}

bool FileGeneratorSettings::addSupportedExtension(fs::path const& extension) noexcept
{
	std::string extensionAsString = extension.string();

	if (!extensionAsString.empty() && extensionAsString[0] == '.' && extension.has_stem())
	{
		_supportedExtensions.emplace(std::move(extensionAsString));

		return true;
	}

	return false;
}

void FileGeneratorSettings::removeToParseFile(fs::path const& path) noexcept
{
	_toParseFiles.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGeneratorSettings::removeToParseDirectory(fs::path const& path) noexcept
{
	_toParseDirectories.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGeneratorSettings::removeIgnoredFile(fs::path const& path) noexcept
{
	_ignoredFiles.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGeneratorSettings::removeSupportedExtension(fs::path const& ext) noexcept
{
	_supportedExtensions.erase(ext.string());
}

void FileGeneratorSettings::removeIgnoredDirectory(fs::path const& path) noexcept
{
	_ignoredDirectories.erase(FilesystemHelpers::sanitizePath(path));
}

void FileGeneratorSettings::clearToParseFiles() noexcept
{
	_toParseFiles.clear();
}

void FileGeneratorSettings::clearToParseDirectories() noexcept
{
	_toParseDirectories.clear();
}

void FileGeneratorSettings::clearIgnoredFiles() noexcept
{
	_ignoredFiles.clear();
}

void FileGeneratorSettings::clearIgnoredDirectories() noexcept
{
	_ignoredDirectories.clear();
}

void FileGeneratorSettings::clearSupportedExtensions() noexcept
{
	_supportedExtensions.clear();
}

void FileGeneratorSettings::loadSupportedExtensions(toml::value const& generationSettings, ILogger* logger) noexcept
{
	//Clear supported extensions before loading
	_supportedExtensions.clear();

	std::unordered_set<std::string> loadedExtensions;
	if (TomlUtility::updateSetting(generationSettings, "supportedExtensions", loadedExtensions, logger) && logger != nullptr)
	{
		for (std::string const& extension : loadedExtensions)
		{
			if (addSupportedExtension(extension))
			{
				if (logger != nullptr)
				{
					logger->log("[TOML] Load new supported extension: " + extension);
				}
			}
			else if (logger != nullptr)
			{
				if (logger != nullptr)
				{
					logger->log("[TOML] Failed to add supported extension: " + extension);
				}
			}
		}
	}
}

void FileGeneratorSettings::loadToParseFiles(toml::value const& generationSettings, ILogger* logger) noexcept
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

void FileGeneratorSettings::loadToParseDirectories(toml::value const& generationSettings, ILogger* logger) noexcept
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

void FileGeneratorSettings::loadIgnoredFiles(toml::value const& generationSettings, ILogger* logger) noexcept
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

void FileGeneratorSettings::loadIgnoredDirectories(toml::value const& generationSettings, ILogger* logger) noexcept
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

std::unordered_set<fs::path, PathHash> const& FileGeneratorSettings::getToParseFiles() const noexcept
{
	return _toParseFiles;
}

std::unordered_set<fs::path, PathHash> const& FileGeneratorSettings::getToParseDirectories() const noexcept
{
	return _toParseDirectories;
}

std::unordered_set<fs::path, PathHash> const& FileGeneratorSettings::getIgnoredFiles() const noexcept
{
	return _ignoredFiles;
}

std::unordered_set<fs::path, PathHash> const& FileGeneratorSettings::getIgnoredDirectories() const noexcept
{
	return _ignoredDirectories;
}

std::unordered_set<std::string> const& FileGeneratorSettings::getSupportedExtensions() const noexcept
{
	return _supportedExtensions;
}