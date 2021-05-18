#include "Kodgen/CodeGen/FileGenerator.h"

#include <fstream>	//fstream

#include "Kodgen/CodeGen/GeneratedFile.h"

using namespace kodgen;

std::set<fs::path> FileGenerator::identifyFilesToProcess(CodeGenUnit const& codeGenUnit, FileGenerationResult& out_genResult, bool forceRegenerateAll) const noexcept
{
	std::set<fs::path> result;

	//Iterate over all "toParseFiles"
	for (fs::path path : settings->getToParseFiles())
	{
		if (fs::exists(path) && !fs::is_directory(path))
		{
			if (forceRegenerateAll || !codeGenUnit.isUpToDate(path))
			{
				result.emplace(path);
			}
			else
			{
				out_genResult.upToDateFiles.push_back(path);
			}
		}
		else
		{
			//Add FileGenerationFile invalid path
			out_genResult.fileGenerationErrors.emplace_back(path, "", "This path was find in the toParseFiles list but it doesn't exist or is not a file.");
			logger->log("File " + path.string() + " doesn't exist", ILogger::ELogSeverity::Warning);
		}
	}

	//Iterate over all "toParseDirectories"
	for (fs::path pathToIncludedDir : settings->getToParseDirectories())
	{
		if (fs::exists(pathToIncludedDir) && fs::is_directory(pathToIncludedDir))
		{
			for (fs::recursive_directory_iterator directoryIt = fs::recursive_directory_iterator(pathToIncludedDir, fs::directory_options::follow_directory_symlink); directoryIt != fs::recursive_directory_iterator(); directoryIt++)
			{
				fs::directory_entry entry = *directoryIt;

				//Just to make sure the entry hasn't been deleted since beginning of directory iteration
				if (entry.exists())
				{
					if (entry.is_regular_file())
					{
						if (settings->supportedExtensions.find(entry.path().extension().string()) != settings->supportedExtensions.cend() &&	//supported extension
							settings->getIgnoredFiles().find(entry.path()) == settings->getIgnoredFiles().cend())								//file is not ignored
						{
							if (forceRegenerateAll || !codeGenUnit.isUpToDate(entry.path()))
							{
								result.emplace(entry.path());
							}
							else
							{
								out_genResult.upToDateFiles.push_back(entry.path());
							}
						}
					}
					else if (entry.is_directory() && settings->getIgnoredDirectories().find(entry.path()) != settings->getIgnoredDirectories().cend())	//directory is ignored
					{
						//Don't iterate on ignored directory content
						directoryIt.disable_recursion_pending();
					}
				}
			}
		}
		else
		{
			//Add FileGenerationFile invalid path
			logger->log("Directory " + pathToIncludedDir.string() + " doesn't exist", ILogger::ELogSeverity::Warning);
		}
	}

	return result;
}

bool FileGenerator::checkOutputDirectory(FileGenerationResult& out_genResult) const noexcept
{
	//Before doing anything, make sure the output directory exists
	//If it doesn't, create it
	if (!fs::exists(settings->getOutputDirectory()))
	{
		//Try to create them is it doesn't exist
		try
		{
			out_genResult.completed = fs::create_directories(settings->getOutputDirectory());

			if (logger != nullptr)
			{
				logger->log("Specified output directory doesn't exist. Create " + FilesystemHelpers::sanitizePath(settings->getOutputDirectory()).string(), ILogger::ELogSeverity::Info);
			}
		}
		catch (fs::filesystem_error const& exception)
		{
			out_genResult.fileGenerationErrors.emplace_back("", "", "Output directory is invalid: " + std::string(exception.what()));

			if (logger != nullptr)
			{
				logger->log("Output directory is invalid: " + std::string(exception.what()), ILogger::ELogSeverity::Error);
			}
		}
	}

	return fs::is_directory(settings->getOutputDirectory());
}

uint32 FileGenerator::getThreadCount(uint32 initialThreadCount) const noexcept
{
	if (initialThreadCount == 0)
	{
		//Use hardware_concurrency if possible
		initialThreadCount = std::thread::hardware_concurrency();

		//If hardware_concurrency hints to 0, use 8 threads
		if (initialThreadCount == 0)
		{
			initialThreadCount = 8u;
		}
	}

	return initialThreadCount;
}

bool FileGenerator::shouldRegenerateFile(fs::path const& filePath) const noexcept
{
	fs::path pathToGeneratedFile = makePathToGeneratedFile(filePath);

	return !fs::exists(pathToGeneratedFile) || fs::last_write_time(filePath) > fs::last_write_time(pathToGeneratedFile);
}

fs::path FileGenerator::makePathToGeneratedFile(fs::path const& sourceFilePath) const noexcept
{
	assert(fs::exists(sourceFilePath) && fs::is_regular_file(sourceFilePath));

	return (settings->getOutputDirectory() / sourceFilePath.filename()).replace_extension(settings->generatedFilesExtension);
}

void FileGenerator::generateMacrosFile(FileParserFactoryBase& fileParserFactory) const noexcept
{
	GeneratedFile macrosDefinitionFile(settings->getOutputDirectory() / settings->entityMacrosFilename);

	PropertyParsingSettings& pps = fileParserFactory.parsingSettings.propertyParsingSettings;

	//Define empty entity macros to allow compilation outside of the Kodgen parsing
	macrosDefinitionFile.writeLines("#pragma once",
									"",
									"#ifndef " + FileParserFactoryBase::parsingMacro,
									"",
									"#define " + pps.namespaceMacroName	+ "(...)",
									"#define " + pps.classMacroName		+ "(...)",
									"#define " + pps.structMacroName	+ "(...)",
									"#define " + pps.variableMacroName	+ "(...)",
									"#define " + pps.fieldMacroName		+ "(...)",
									"#define " + pps.methodMacroName	+ "(...)",
									"#define " + pps.enumMacroName		+ "(...)",
									"#define " + pps.enumValueMacroName	+ "(...)",
									"#define " + pps.functionMacroName	+ "(...)");

	macrosDefinitionFile.writeLine("\n#endif");
}

void FileGenerator::setupFileGenerationUnit(CodeGenUnit& codeGenUnit) const noexcept
{
	codeGenUnit.logger	= logger;
	//fileGenerationUnit.settings	= &settings;
}

void FileGenerator::checkGenerationSettings() const noexcept
{
	auto& ignoredDirectories = settings->getIgnoredDirectories();

	//Emit a warning if the output directory content is going to be parsed
	if (fs::exists(settings->getOutputDirectory()) &&											//abort check if the output directory doesn't exist
		!fs::is_empty(settings->getOutputDirectory()) &&											//abort check if the output directory is empty
		ignoredDirectories.find(settings->getOutputDirectory()) == ignoredDirectories.cend())	//abort check if the output directory is already ignored
	{
		for (fs::path const& parsedDirectory : settings->getToParseDirectories())
		{
			if (FilesystemHelpers::isChildPath(settings->getOutputDirectory(), parsedDirectory))
			{
				if (logger != nullptr)
				{
					logger->log("Output directory is contained in a parsed directory, hence generated files will be parsed. If this is not intended, add the output directory to the list of ignored directories.", ILogger::ELogSeverity::Warning);
				}

				break;
			}
		}
	}
}