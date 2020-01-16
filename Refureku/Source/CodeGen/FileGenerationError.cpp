#include "CodeGen/FileGenerationError.h"

using namespace refureku;

FileGenerationError::FileGenerationError(fs::path const& filepath, std::string const& entityName, FileGenerationError::EEntityType entityType, EFileGenerationError fileGenError) noexcept:
	_filename{filepath.string()},
	_entityName{entityName},
	_entityType{entityType},
	_fileGenError{fileGenError}
{
}

std::string const& FileGenerationError::getFilename() const noexcept
{
	return _filename;
}

std::string const& FileGenerationError::getEntityName() const noexcept
{
	return _entityName;
}

FileGenerationError::EEntityType FileGenerationError::getEntityType() const noexcept
{
	return _entityType;
}

EFileGenerationError FileGenerationError::getFileGenerationError() const noexcept
{
	return _fileGenError;
}