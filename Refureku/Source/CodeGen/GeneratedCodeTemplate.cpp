#include "CodeGen/GeneratedCodeTemplate.h"

#include <assert.h>

using namespace refureku;

void GeneratedCodeTemplate::setWritingStream(std::ofstream* writingFile) noexcept
{
	_writingFile = writingFile;
}

void GeneratedCodeTemplate::write(std::string&& text) noexcept
{
	assert(_writingFile != nullptr);

	*_writingFile << text << std::endl;
}