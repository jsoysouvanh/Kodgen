#include "CodeGen/GeneratedCodeTemplate.h"

#include <assert.h>

using namespace refureku;

void GeneratedCodeTemplate::setWritingStream(std::ofstream* writingFile) noexcept
{
	_writingFile = writingFile;
}

void GeneratedCodeTemplate::write(std::string const& line) noexcept
{
	assert(_writingFile != nullptr);

	*_writingFile << line << std::endl;
}

void GeneratedCodeTemplate::write(std::string&& line) noexcept
{
	assert(_writingFile != nullptr);

	*_writingFile << std::forward<std::string>(line) << std::endl;
}

void GeneratedCodeTemplate::expandWriteMacroLines(std::string&& line) noexcept
{
	write("\t" + std::forward<std::string>(line));
	write("");
}