#include "CodeGen/FileGenerator.h"

#include <assert.h>

using namespace refureku;

FileGenerator::FileGenerator() noexcept
{

}

FileGenerator::~FileGenerator() noexcept
{
	for (auto& [key, value] : _generatedCodeTemplates)
	{
		delete value;
	}
}

void FileGenerator::updateSupportedCodeTemplateRegex() noexcept
{
	_supportedCodeTemplateRegex.clear();

	for (auto& [key, value] : _generatedCodeTemplates)
	{
		_supportedCodeTemplateRegex += key + "|";
	}

	_supportedCodeTemplateRegex.pop_back();
}

void FileGenerator::generateEntityFile(FileGenerationResult& genResult, fs::path const& filePath, ParsingResult const& parsingResult) noexcept
{
	GeneratedFile generatedFile(filePath, makePathToGeneratedFile(filePath));

	//Header
	writeHeader(generatedFile, parsingResult);

	//Actual file content (per entity)
	for (StructClassInfo structOrClassInfo : parsingResult.classes)
	{
		writeEntityToFile(generatedFile, structOrClassInfo, genResult, true);
	}

	for (EnumInfo enumInfo : parsingResult.enums)
	{
		writeEntityToFile(generatedFile, enumInfo, genResult, false);
	}

	//Footer
	writeFooter(generatedFile, parsingResult);
}

GeneratedCodeTemplate* FileGenerator::getEntityGeneratedCodeTemplate(EntityInfo& entityInfo, bool isClass, EFileGenerationError& out_error) const noexcept
{
	GeneratedCodeTemplate* result = nullptr;

	//Find the specified code template
	std::set<ComplexProperty>::const_iterator it = std::find_if(entityInfo.properties.complexProperties.cbegin(), entityInfo.properties.complexProperties.cend(),
																[this](ComplexProperty const& prop) { return prop.name == codeTemplateMainComplexPropertyName; });

	if (it == entityInfo.properties.complexProperties.cend())	//No main property corresponding to codeTemplateMainComplexPropertyName found
	{
		if (isClass)
		{
			if (_defaultClassTemplate != nullptr)
			{
				result = _defaultClassTemplate;
			}
			else
			{
				out_error = EFileGenerationError::MissingGeneratedCodeTemplateComplexProperty;
			}
		}
		else	//isEnum
		{
			if (_defaultEnumTemplate != nullptr)
			{
				result = _defaultEnumTemplate;
			}
			else
			{
				out_error = EFileGenerationError::MissingGeneratedCodeTemplateComplexProperty;
			}
		}
	}
	else if (it->subProperties.empty())	//No sub prop provided to the codeTemplateMainComplexPropertyName main prop
	{
		out_error = EFileGenerationError::NoGeneratedCodeTemplateProvided;
	}
	else if (it->subProperties.size() > 1)	//More than one prop provided to the codeTemplateMainComplexPropertyName main prop
	{
		out_error = EFileGenerationError::TooManyGeneratedCodeTemplateProvided;
	}
	else
	{
		std::unordered_map<std::string,	GeneratedCodeTemplate*>::const_iterator it2 = _generatedCodeTemplates.find(it->subProperties[0]);

		if (it2 != _generatedCodeTemplates.cend())
		{
			result = it2->second;
		}
		else
		{
			out_error = EFileGenerationError::UnregisteredGeneratedCodeTemplateProvided;
		}
	}

	//make sure an error is set if we have no result
	assert(result != nullptr || out_error != EFileGenerationError::Count);

	return result;
}

void FileGenerator::writeEntityToFile(GeneratedFile& generatedFile, EntityInfo& entityInfo, FileGenerationResult& genResult, bool isClass) noexcept
{
	EFileGenerationError error = EFileGenerationError::Count;
	GeneratedCodeTemplate* codeTemplate = getEntityGeneratedCodeTemplate(entityInfo, isClass, error);

	if (codeTemplate != nullptr)
	{
		codeTemplate->generateCode(generatedFile, entityInfo);
	}
	else
	{
		genResult.fileGenerationErrors.emplace_back(FileGenerationError(generatedFile.getSourceFile(), entityInfo.name, error));
	}
}

bool FileGenerator::shouldRegenerateFile(fs::path const& filePath) const noexcept
{
	fs::path pathToGeneratedFile = makePathToGeneratedFile(filePath);

	return !fs::exists(pathToGeneratedFile) || fs::last_write_time(filePath) > fs::last_write_time(pathToGeneratedFile);
}

fs::path FileGenerator::makePathToGeneratedFile(fs::path const& sourceFilePath) const noexcept
{
	return (outputDirectory / sourceFilePath.filename()).replace_extension(generatedFilesExtension);
}

void FileGenerator::writeHeader(GeneratedFile& file, ParsingResult const& parsingResult) const noexcept
{
	file.writeLine("#pragma once\n");
}

void FileGenerator::writeFooter(GeneratedFile& file, ParsingResult const& parsingResult) const noexcept
{
	//Default implementation has no footer
}

bool FileGenerator::addFile(fs::path filePath) noexcept
{
	if (fs::exists(filePath) && !fs::is_directory(filePath))
	{
		return _includedFiles.insert(std::move(filePath)).second;
	}

	return false;
}

bool FileGenerator::addDirectory(fs::path dirPath) noexcept
{
	if (fs::exists(dirPath) && fs::is_directory(dirPath))
	{
		return _includedDirectories.insert(std::move(dirPath)).second;
	}

	return false;
}

void FileGenerator::addGeneratedCodeTemplate(std::string templateName, GeneratedCodeTemplate* codeTemplate) noexcept
{
	_generatedCodeTemplates[templateName] = codeTemplate;

	updateSupportedCodeTemplateRegex();
}

bool FileGenerator::setDefaultClassTemplate(std::string templateName) noexcept
{
	decltype(_generatedCodeTemplates)::const_iterator it = _generatedCodeTemplates.find(templateName);

	if (it != _generatedCodeTemplates.cend())
	{
		_defaultClassTemplate = it->second;
		
		return true;
	}

	return false;
}

bool FileGenerator::setDefaultEnumTemplate(std::string templateName) noexcept
{
	decltype(_generatedCodeTemplates)::const_iterator it = _generatedCodeTemplates.find(templateName);

	if (it != _generatedCodeTemplates.cend())
	{
		_defaultEnumTemplate = it->second;

		return true;
	}

	return false;
}

FileGenerationResult FileGenerator::generateFiles(Parser& parser, bool forceRegenerateAll) noexcept
{
	FileGenerationResult	genResult;
	bool					parsingSuccess;

	//Before doing anything, make sure destination folder exists
	if (!fs::exists(outputDirectory))
		genResult.completed = fs::create_directories(outputDirectory);

	if (fs::is_directory(outputDirectory))
	{
		//Make sure the CodeTemplate property is setup in class, struct and enum
		parser.parsingSettings.propertyParsingSettings.classPropertyRules.removeComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName));
		parser.parsingSettings.propertyParsingSettings.classPropertyRules.addComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName), std::string(_supportedCodeTemplateRegex));

		parser.parsingSettings.propertyParsingSettings.structPropertyRules.removeComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName));
		parser.parsingSettings.propertyParsingSettings.structPropertyRules.addComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName), std::string(_supportedCodeTemplateRegex));

		parser.parsingSettings.propertyParsingSettings.enumPropertyRules.removeComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName));
		parser.parsingSettings.propertyParsingSettings.enumPropertyRules.addComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName), std::string(_supportedCodeTemplateRegex));

		for (fs::path const& path : _includedFiles)
		{
			if (forceRegenerateAll || shouldRegenerateFile(path))
			{
				parser.clear();

				//Parse file
				ParsingResult parsingResult;
				parsingSuccess = parser.parse(path, parsingResult);

				if (parsingSuccess)
				{
					generateEntityFile(genResult, path, parsingResult);
				}
				else
				{
					//Transfer parsing errors into the file generation result
					genResult.parsingErrors.insert(genResult.parsingErrors.end(), std::make_move_iterator(parsingResult.parsingErrors.begin()), std::make_move_iterator(parsingResult.parsingErrors.end()));
				}
			}
		}

		genResult.completed = true;
	}

	return genResult;
}