#include "ParsingError.h"

#include "Helpers.h"

using namespace refureku;

ParsingError::ParsingError(CXSourceLocation errorSourceLocation, PropertyParsingError ppe) noexcept:
	_line{0},
	_column{0},
	_filename{""},
	_propertyParsingError{ppe}
{
	CXFile file;

	clang_getExpansionLocation(errorSourceLocation, &file, &_line, &_column, nullptr);

	_filename = Helpers::getString(clang_getFileName(file));
}

std::string const& ParsingError::getFilename() const noexcept
{
	return _filename;
}

unsigned ParsingError::getLine() const noexcept
{
	return _line;
}

unsigned ParsingError::getColumn() const noexcept
{
	return _column;
}

PropertyParsingError ParsingError::getErrorValue() const noexcept
{
	return _propertyParsingError;
}