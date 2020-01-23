#include "CodeGen/GeneratedCodeTemplate.h"

#include <assert.h>

using namespace refureku;

void GeneratedCodeTemplate::setWritingStream(std::ofstream* writingFile) noexcept
{
	_writingFile = writingFile;
}

void GeneratedCodeTemplate::writeLine(std::string const& line) noexcept
{
	*_writingFile << line << std::endl;
}

void GeneratedCodeTemplate::writeLine(std::string&& line) noexcept
{
	*_writingFile << std::forward<std::string>(line) << std::endl;
}

void GeneratedCodeTemplate::writeLines(std::string const& line) noexcept
{
	writeLine(line);
}

void GeneratedCodeTemplate::writeLines(std::string&& line) noexcept
{
	writeLine(std::forward<std::string>(line));
}

void GeneratedCodeTemplate::expandWriteMacroLines(std::string&& line) noexcept
{
	writeLine("\t" + std::forward<std::string>(line));
	writeLine("");
}