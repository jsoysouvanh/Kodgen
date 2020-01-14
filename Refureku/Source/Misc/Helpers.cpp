#include "Misc/Helpers.h"

using namespace refureku;

std::string Helpers::getString(CXString&& clangString) noexcept
{
	std::string result = clang_getCString(clangString);
	clang_disposeString(clangString);

	return result;
}