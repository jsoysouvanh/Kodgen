#include "CodeGen/GeneratedFile.h"

using namespace refureku;

GeneratedFile::GeneratedFile(fs::path const& sourceFile, fs::path&& generatedFilePath) noexcept:
	_streamToFile(generatedFilePath.string(), std::ios::out | std::ios::trunc),
	_sourceFile{sourceFile}
{
}

GeneratedFile::~GeneratedFile() noexcept
{
	_streamToFile.close();
}

void GeneratedFile::writeLine(std::string const& line) noexcept
{
	_streamToFile << line << std::endl;
}

void GeneratedFile::writeLine(std::string&& line) noexcept
{
	_streamToFile << std::forward<std::string>(line) << std::endl;
}

void GeneratedFile::writeLines(std::string const& line) noexcept
{
	writeLine(line);
}

void GeneratedFile::writeLines(std::string&& line) noexcept
{
	writeLine(std::forward<std::string>(line));
}

void GeneratedFile::expandWriteMacroLines(std::string&& line) noexcept
{
	writeLine("\t" + std::forward<std::string>(line));
	writeLine("");
}

fs::path const& GeneratedFile::getSourceFile() const noexcept
{
	return _sourceFile;
}