#include "CodeGen/FileGenerator.h"



using namespace refureku;

FileGenerator::FileGenerator() noexcept
{

}

FileGenerator::~FileGenerator() noexcept
{

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

bool FileGenerator::generateFiles(Parser& parser, bool forceRegenerateAll) noexcept
{
	bool success;

	for (fs::path path : _includedFiles)
	{
		parser.clear();
		
		//Parse file
		ParsingResult result;
		success = parser.parse(path, result);

		if (success)
		{
			for (ClassInfo classInfo : result.classes)
			{
				//Find the specified code template
				std::set<ComplexProperty>::const_iterator it = std::find_if(classInfo.properties.complexProperties.cbegin(), classInfo.properties.complexProperties.cend(),
																			[](ComplexProperty const& prop) { return prop.name == "CodeTemplate"; });
				
				if (it == classInfo.properties.complexProperties.cend())
				{
					if (_defaultCodeTemplate != nullptr)
					{
						_defaultCodeTemplate->generateCode(path, classInfo);
						_defaultCodeTemplate->releaseGeneratedFile();
					}
					else
					{
						std::cout << "Missing CodeTemplate property and no default code template is specified, can't generate file";
						continue;
					}
				}
				else if (it->subProperties.empty())
				{
					std::cout << "No CodeTemplate provided";
					continue;
				}
				else if (it->subProperties.size() > 1)
				{
					std::cout << "Too many CodeTemplate subprops provided, only 1 is expected";
					continue;
				}
				else
				{
					std::unordered_map<std::string,	GeneratedCodeTemplate*>::const_iterator it2 = _generatedCodeTemplates.find(it->subProperties[0]);

					if (it2 != _generatedCodeTemplates.cend())
					{
						it2->second->generateCode(path, classInfo);
						it2->second->releaseGeneratedFile();
					}
					else
					{
						std::cout << "Unsupported CodeTemplate";
					}
				}
			}
		}
	}

	return true;
}