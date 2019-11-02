#pragma once

#include <set>
#include <filesystem>

namespace refureku
{
	class FileGenerator
	{
		private:
			#pragma region Variables

			std::set<std::filesystem::path>	_includedFiles;
			std::set<std::filesystem::path>	_includedDirectories;

			#pragma endregion

		protected:

		public:
			#pragma region Constructors/Destructor

			FileGenerator() noexcept;
			~FileGenerator() noexcept;

			#pragma endregion

			#pragma region Methods

			/**
			*	Add a file to the list of files to parse.
			*/
			void AddFile(std::filesystem::path const& filePath) noexcept;

			/**
			*	Add a directory to the list of directories to parse.
			*	All directories contained in the given directory will be recursively inspected.
			*	All files contained in any included directory will be parsed.
			*/
			void AddDirectory(std::filesystem::path const& dirPath) noexcept;

			#pragma endregion
	};
}