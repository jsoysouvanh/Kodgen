#include "CodeGen/GeneratedCodeTemplate.h"

using namespace refureku;

void GeneratedCodeTemplate::generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo) noexcept
{
	_writingFile.open(getGeneratedFilename(parsedFile, entityInfo).string(), std::ios::out | std::ios::trunc);
}

void GeneratedCodeTemplate::releaseGeneratedFile() noexcept
{
	if (_writingFile.is_open())
	{
		_writingFile.close();
	}
}

void GeneratedCodeTemplate::write(std::string&& text) noexcept
{
	_writingFile << text;
}