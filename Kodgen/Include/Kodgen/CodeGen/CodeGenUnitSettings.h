/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/Misc/Settings.h"
#include "Kodgen/Misc/Filesystem.h"

namespace kodgen
{
	class CodeGenUnitSettings : public Settings
	{
		private:
			/**
			*	Path to the directory in which all files should be generated (and where existing ones are).
			*	If the output directory doesn't exist, it will be created if possible during the code generation process.
			*/
			fs::path	_outputDirectory;

		protected:
			/** Toml section name containing settings for CodeGenUnitSettings. */
			static constexpr char const*	tomlSectionName = "CodeGenUnit";

			/**
			*	@brief Load all settings from the provided toml data.
			* 
			*	@param tomlData	Data loaded from a toml file.
			*	@param logger	Optional logger used to issue loading logs. Can be nullptr.
			* 
			*	@return true if no error occured during loading, else false.
			*/
			virtual bool	loadSettingsValues(toml::value const&	tomlData,
											   ILogger*				logger)				noexcept override;

			/**
			*	@brief Load the outputDirectory setting from toml.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadOutputDirectory(toml::value const&	generationSettings,
												ILogger*			logger)						noexcept;

			/**
			*	@brief Load the outputDirectory setting from toml.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadEntityMacrosFilename(toml::value const&	generationSettings,
													 ILogger*			logger)					noexcept;

		public:
			/** Name of the header containing all entity macro definitions. */
			static inline fs::path const entityMacrosFilename	= "EntityMacros.h";

			/**
			*	@brief	Setter for _outputDirectory.
			*			If the path exists check that it is a directory.
			*			Sanitize the provided path before setting.
			*	
			*	@param outputDirectory New output directory path.
			*
			*	@return true if the outputDirectory has been updated, else false.
			*/
			bool			setOutputDirectory(fs::path outputDirectory)		noexcept;

			/**
			*	@brief Getter for _outputDirectory.
			*
			*	@return _outputDirectory.
			*/
			fs::path const&	getOutputDirectory()						const	noexcept;
	};
}