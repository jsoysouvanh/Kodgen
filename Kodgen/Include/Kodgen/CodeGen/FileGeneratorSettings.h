/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <unordered_set>
#include <string>

#include "Kodgen/Misc/Settings.h"
#include "Kodgen/Misc/Filesystem.h"

namespace kodgen
{
	class FileGeneratorSettings : public Settings
	{
		private:
			/** Section name used in the toml file for FileGenerator settings. */
			static constexpr char const*	_tomlSectionName	= "FileGenerator";

			/**
			*	Collection of files to parse.
			*	These files will be parsed without any further check if they exist.
			*/
			std::unordered_set<fs::path, PathHash>	_toParseFiles;

			/**
			*	Collection of directories containing files to parse.
			*	All directories contained in the given directories will be recursively inspected, except if they are ignored.
			*	All files contained in any parsed directory will be parsed, except if they are ignored or if their extension is not contained in the supportedExtensions.
			*/
			std::unordered_set<fs::path, PathHash>	_toParseDirectories;

			/**
			*	Collection of ignored files.
			*	These files will never be parsed (except if they are also part of the includedFiles collection).
			*/
			std::unordered_set<fs::path, PathHash>	_ignoredFiles;

			/**
			*	Collection of ignored directories.
			*	All directories contained in the given directories will be ignored, except if they are included.
			*	All files contained in any ignored directory will be ignored, except if they are included.
			*/
			std::unordered_set<fs::path, PathHash>	_ignoredDirectories;

			/** Extensions of files which should be considered for parsing. */
			std::unordered_set<std::string>			_supportedExtensions;

		protected:
			/**
			*	@brief Load all settings from the provided toml data.
			* 
			*	@param tomlData	Data loaded from a toml file.
			*	@param logger	Optional logger used to issue loading logs. Can be nullptr.
			* 
			*	@return true if no error occured during loading, else false.
			*/
			virtual bool	loadSettingsValues(toml::value const&	tomlData,
											   ILogger*			logger)		noexcept override;

			/**
			*	@brief	Load the supportedExtensions setting from toml.
			*			Loaded extensions completely replace previously supported extensions if any.
			*
			*	@param generationSettings Toml content.
			*/
			void	loadSupportedExtensions(toml::value const&	generationSettings,
											ILogger*			logger)					noexcept;

			/**
			*	@brief	Load the toParseFiles setting from toml.
			*			Loaded files completely replace previous toParseFiles if any.
			*
			*	@param generationSettings Toml content.
			*/
			void	loadToParseFiles(toml::value const&	generationSettings,
									 ILogger*			logger)							noexcept;

			/**
			*	@brief	Load the toParseDirectories setting from toml.
			*			Loaded directories completely replace previous toParseDirectories if any.
			*
			*	@param generationSettings Toml content.
			*/
			void	loadToParseDirectories(toml::value const&	generationSettings,
										   ILogger*				logger)					noexcept;

			/**
			*	@brief	Load the ignoredFiles setting from toml.
			*			Loaded files completely replace previous ignoredFiles if any.
			*
			*	@param generationSettings Toml content.
			*/
			void	loadIgnoredFiles(toml::value const&	generationSettings,
									 ILogger*			logger)							noexcept;

			/**
			*	@brief	Load the ignoredDirectories setting from toml.
			*			Loaded directories completely replace previous ignoredDirectories if any.
			*
			*	@param generationSettings Toml content.
			*/
			void	loadIgnoredDirectories(toml::value const&	generationSettings,
										   ILogger*				logger)					noexcept;

		public:
			/**
			*	@brief	Add a file to the list of parsed files.
			*			If the path is invalid, doesn't exist, is not a file, or is already in the list, abort the operation.
			*	
			*	@param path Path to the file to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addToParseFile(fs::path const& path)				noexcept;

			/**
			*	@brief	Add a directory to the list of parsed directories.
			*			If the path is invalid, doesn't exist, is not a directory, or is already in the list, abort the operation.
			*	
			*	@param path Path to the directory to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addToParseDirectory(fs::path const& path)			noexcept;

			/**
			*	@brief	Add a file to the list of ignored files.
			*			If the path is invalid, doesn't exist, is not a file, or is already in the list, abort the operation.
			*	
			*	@param path Path to the file to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addIgnoredFile(fs::path const& path)				noexcept;

			/**
			*	@brief	Add a directory to the list of ignored directories.
			*			If the path is invalid, doesn't exist, is not a directory, or is already in the list, abort the operation.
			*	
			*	@param path Path to the directory to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addIgnoredDirectory(fs::path const& path)			noexcept;

			/**
			*	@brief	Add a file extension to the list of supported file extensions.
			*			A valid file extension must start with '.' followed by characters.
			*	
			*	@param ext Extension to add.
			*	
			*	@return true if the extension has been added successfuly, else false.
			*/
			bool addSupportedExtension(fs::path const& extension)	noexcept;

			/**
			*	@brief Remove a file from the list of parsed files.
			*	
			*	@param path Path to the file to remove.
			*/
			void removeToParseFile(fs::path const& path)			noexcept;

			/**
			*	@brief Remove a directory from the list of parsed directories.
			*	
			*	@param path Path to the directory to remove.
			*/
			void removeToParseDirectory(fs::path const& path)		noexcept;

			/**
			*	@brief Remove a file from the list of ignored files.
			*	
			*	@param path Path to the file to remove.
			*/
			void removeIgnoredFile(fs::path const& path)			noexcept;

			/**
			*	@brief	Remove a file extension from the list of supported file extensions.
			*	
			*	@param ext Extension to remove.
			*/
			void removeSupportedExtension(fs::path const& ext)		noexcept;

			/**
			*	@brief Remove a directory from the list of ignored directories.
			*	
			*	@param path Path to the directory to remove.
			*/
			void removeIgnoredDirectory(fs::path const& path)		noexcept;

			/**
			*	@brief Clear the list of files to parse.
			*/
			void clearToParseFiles()								noexcept;

			/**
			*	@brief Clear the list of directories to parse.
			*/
			void clearToParseDirectories()							noexcept;

			/**
			*	@brief Clear the list of ignored files.
			*/
			void clearIgnoredFiles()								noexcept;

			/**
			*	@brief Clear the list of ignored directories.
			*/
			void clearIgnoredDirectories()							noexcept;

			/**
			*	@brief Clear the list of supported extensions.
			*/
			void clearSupportedExtensions()							noexcept;

			/**
			*	@brief Getter for _toParseFiles.
			*	
			*	@return _toParseFiles.
			*/
			std::unordered_set<fs::path, PathHash> const&	getToParseFiles()			const	noexcept;

			/**
			*	@brief Getter for _toParseDirectories.
			*	
			*	@return _toParseDirectories.
			*/
			std::unordered_set<fs::path, PathHash> const&	getToParseDirectories()		const	noexcept;

			/**
			*	@brief Getter for _ignoredFiles.
			*	
			*	@return _ignoredFiles.
			*/
			std::unordered_set<fs::path, PathHash> const&	getIgnoredFiles()			const	noexcept;

			/**
			*	@brief Getter for _ignoredDirectories.
			*	
			*	@return _ignoredDirectories.
			*/
			std::unordered_set<fs::path, PathHash> const&	getIgnoredDirectories()		const	noexcept;

			/**
			*	@brief Getter for _supportedExtensions.
			*	
			*	@return _supportedExtensions.
			*/
			std::unordered_set<std::string> const&			getSupportedExtensions()	const	noexcept;
	};
}