#pragma once

#include <string>

#include "Misc/SafeFilesystem.h"
#include "CodeGen/EFileGenerationError.h"

namespace refureku
{
	class FileGenerationError
	{
		public:
			enum class EEntityType
			{
				Class,
				Enum
			};

		private:
			std::string				_filename		= "";
			std::string				_entityName		= "";
			EEntityType				_entityType		= EEntityType::Class;
			EFileGenerationError	_fileGenError	= EFileGenerationError::Count;

		public:
			FileGenerationError()							= delete;
			FileGenerationError(fs::path const& filepath, std::string const& entityName, EEntityType entityType, EFileGenerationError fileGenError) noexcept;
			FileGenerationError(FileGenerationError const&) = default;
			FileGenerationError(FileGenerationError&&)		= default;

			std::string const&		getFilename()				const	noexcept;
			std::string const&		getEntityName()				const	noexcept;
			EEntityType				getEntityType()				const	noexcept;
			EFileGenerationError	getFileGenerationError()	const	noexcept;

			FileGenerationError& operator=(FileGenerationError const&)	= default;
			FileGenerationError& operator=(FileGenerationError&&)		= default;
	};
}