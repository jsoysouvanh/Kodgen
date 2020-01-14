#pragma once

#include <set>
#include <string>

#include "Misc/SafeFilesystem.h"

namespace refureku
{
	class FileGenerator
	{
		private:
			std::set<fs::path>		_includedFiles;
			std::set<fs::path>		_includedDirectories;
			//std::set<std::string>	_;

		protected:

		public:
			FileGenerator() noexcept;
			~FileGenerator() noexcept;

			/**
			*	@brief Add a file to the list of files to parse.
			*
			*	@return true on success (the path exists and is a file), else false
			*/

			bool AddFile(fs::path filePath) noexcept;

			/**
			*	@brief Add a directory to the list of directories to parse.
			*	@brief All directories contained in the given directory will be recursively inspected.
			*	@brief All files contained in any included directory will be parsed.
			*
			*	@return true on success (the path exists and is a directory), else false
			*/
			bool AddDirectory(fs::path dirPath) noexcept;
	};
}