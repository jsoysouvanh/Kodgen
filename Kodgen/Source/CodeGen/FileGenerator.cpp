#include "Kodgen/CodeGen/FileGenerator.h"

#include "Kodgen/CodeGen/FileGeneratorSettings.h"
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
			if (!codeGenUnit.isUpToDate(path) || forceRegenerateAll)
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
			logger->log("File " + path.string() + " found in FileGeneratorSettings::toParseFiles doesn't exist. Skip.", ILogger::ELogSeverity::Warning);
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
						if (settings->getSupportedExtensions().find(entry.path().extension().string()) != settings->getSupportedExtensions().cend() &&	//supported extension
							settings->getIgnoredFiles().find(entry.path()) == settings->getIgnoredFiles().cend())								//file is not ignored
						{
							if (!codeGenUnit.isUpToDate(entry.path()) || forceRegenerateAll)
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
			logger->log("Directory " + pathToIncludedDir.string() + " found in FileGeneratorSettings::toParseDirectories doesn't exist. Skip.", ILogger::ELogSeverity::Warning);
		}
	}

	return result;
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

void FileGenerator::generateMacrosFile(FileParserFactoryBase& fileParserFactory, CodeGenUnit const& codeGenUnit) const noexcept
{
	GeneratedFile macrosDefinitionFile(codeGenUnit.getSettings()->getOutputDirectory() / CodeGenUnitSettings::entityMacrosFilename);

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

bool FileGenerator::checkGenerationSetup(CodeGenUnit const& codeGenUnit) const noexcept
{
	bool canLog	= logger != nullptr;
	bool result	= codeGenUnit.checkSettings();
	
	if (settings == nullptr)
	{
		if (canLog)
		{
			logger->log("FileGenerator settings have not been set.", ILogger::ELogSeverity::Error);
		}

		result &= false;
	}
	else if (CodeGenUnitSettings const* codeGenUnitSettings = codeGenUnit.getSettings())
	{
		auto const& ignoredDirectories = settings->getIgnoredDirectories();

		//Emit a warning if the output directory content is going to be parsed
		if (fs::exists(codeGenUnitSettings->getOutputDirectory()) &&											//abort check if the output directory doesn't exist
			!fs::is_empty(codeGenUnitSettings->getOutputDirectory()) &&										//abort check if the output directory contains no file
			ignoredDirectories.find(codeGenUnitSettings->getOutputDirectory()) == ignoredDirectories.cend())	//abort check if the output directory is already ignored
		{
			for (fs::path const& parsedDirectory : settings->getToParseDirectories())
			{
				if (FilesystemHelpers::isChildPath(codeGenUnitSettings->getOutputDirectory(), parsedDirectory))
				{
					if (canLog)
					{
						logger->log("Output directory is contained in a parsed directory, hence generated files will be parsed. If this is not intended, add the output directory to the list of ignored directories.", ILogger::ELogSeverity::Warning);
					}

					break;
				}
			}
		}
	}
	
	return result;
}