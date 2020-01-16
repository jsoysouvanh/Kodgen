#include "CodeGen/FileGenerator.h"

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

void FileGenerator::generateClassFiles(FileGenerationResult& genResult, fs::path const& filePath, ParsingResult const& parsingResult) noexcept
{
	for (ClassInfo classInfo : parsingResult.classes)
	{
		//Find the specified code template
		std::set<ComplexProperty>::const_iterator it = std::find_if(classInfo.properties.complexProperties.cbegin(), classInfo.properties.complexProperties.cend(),
																	[this](ComplexProperty const& prop) { return prop.name == codeTemplateMainComplexPropertyName; });

		if (it == classInfo.properties.complexProperties.cend())
		{
			if (_defaultClassTemplate != nullptr)
			{
				_defaultClassTemplate->generateCode(filePath, classInfo);
				_defaultClassTemplate->releaseGeneratedFile();
			}
			else
			{
				genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, classInfo.name, FileGenerationError::EEntityType::Class, EFileGenerationError::MissingGeneratedCodeTemplateComplexProperty));
			}
		}
		else if (it->subProperties.empty())
		{
			genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, classInfo.name, FileGenerationError::EEntityType::Class, EFileGenerationError::NoGeneratedCodeTemplateProvided));
		}
		else if (it->subProperties.size() > 1)
		{
			genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, classInfo.name, FileGenerationError::EEntityType::Class, EFileGenerationError::TooManyGeneratedCodeTemplateProvided));
		}
		else
		{
			std::unordered_map<std::string,	GeneratedCodeTemplate*>::const_iterator it2 = _generatedCodeTemplates.find(it->subProperties[0]);

			if (it2 != _generatedCodeTemplates.cend())
			{
				it2->second->generateCode(filePath, classInfo);
				it2->second->releaseGeneratedFile();
			}
			else
			{
				genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, classInfo.name, FileGenerationError::EEntityType::Class, EFileGenerationError::UnregisteredGeneratedCodeTemplateProvided));
			}
		}
	}
}

void FileGenerator::generateEnumFiles(FileGenerationResult& genResult, fs::path const& filePath, ParsingResult const& parsingResult) noexcept
{
	for (EnumInfo enumInfo : parsingResult.enums)
	{
		//Find the specified code template
		std::set<ComplexProperty>::const_iterator it = std::find_if(enumInfo.properties.complexProperties.cbegin(), enumInfo.properties.complexProperties.cend(),
																	[this](ComplexProperty const& prop) { return prop.name == codeTemplateMainComplexPropertyName; });

		if (it == enumInfo.properties.complexProperties.cend())
		{
			if (_defaultClassTemplate != nullptr)
			{
				_defaultClassTemplate->generateCode(filePath, enumInfo);
				_defaultClassTemplate->releaseGeneratedFile();
			}
			else
			{
				genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, enumInfo.name, FileGenerationError::EEntityType::Enum, EFileGenerationError::MissingGeneratedCodeTemplateComplexProperty));
			}
		}
		else if (it->subProperties.empty())
		{
			genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, enumInfo.name, FileGenerationError::EEntityType::Enum, EFileGenerationError::NoGeneratedCodeTemplateProvided));
		}
		else if (it->subProperties.size() > 1)
		{
			genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, enumInfo.name, FileGenerationError::EEntityType::Enum, EFileGenerationError::TooManyGeneratedCodeTemplateProvided));
		}
		else
		{
			std::unordered_map<std::string,	GeneratedCodeTemplate*>::const_iterator it2 = _generatedCodeTemplates.find(it->subProperties[0]);

			if (it2 != _generatedCodeTemplates.cend())
			{
				it2->second->generateCode(filePath, enumInfo);
				it2->second->releaseGeneratedFile();
			}
			else
			{
				genResult.fileGenerationErrors.emplace_back(FileGenerationError(filePath, enumInfo.name, FileGenerationError::EEntityType::Enum, EFileGenerationError::UnregisteredGeneratedCodeTemplateProvided));
			}
		}
	}
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

	//Make sure the CodeTemplate property is setup
	parser.parsingSettings.propertyParsingSettings.classPropertyRules.removeComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName));
	parser.parsingSettings.propertyParsingSettings.classPropertyRules.addComplexPropertyRule(std::string(codeTemplateMainComplexPropertyName), std::string(_supportedCodeTemplateRegex));

	for (fs::path path : _includedFiles)
	{
		if (forceRegenerateAll || true /* TODO FILE DOESN'T EXIST YET */ || true /* TODO CHECK DATE TIME */)
		{
			parser.clear();

			//Parse file
			ParsingResult parsingResult;
			parsingSuccess = parser.parse(path, parsingResult);

			if (parsingSuccess)
			{
				generateClassFiles(genResult, path, parsingResult);
				generateEnumFiles(genResult, path, parsingResult);
			}
			else
			{
				//Transfer parsing errors into the file generation result
				genResult.parsingErrors.insert(genResult.parsingErrors.end(), std::make_move_iterator(parsingResult.parsingErrors.begin()), std::make_move_iterator(parsingResult.parsingErrors.end()));
			}
		}
	}

	return genResult;
}