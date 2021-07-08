/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/Misc/Toml11.h"
#include "Kodgen/Misc/Filesystem.h"

namespace kodgen
{
	//Forward declaration
	class ILogger;

	class Settings
	{
		protected:
			/**
			*	@brief Load all settings from the provided toml data.
			* 
			*	@param tomlData	Data loaded from a toml file.
			*	@param logger	Optional logger used to issue loading logs. Can be nullptr.
			* 
			*	@return true if no error occured during loading, else false.
			*/
			virtual bool loadSettingsValues(toml::value const&	tomlData,
											ILogger*			logger)		noexcept = 0;

		public:
			virtual ~Settings() = default;

			/**
			*	@brief	Setup this object's member variables with the provided toml file.
			*			Member variables not specified in the toml file remain unchanged.
			*
			*	@param pathToSettingsFile	Path to the toml file.
			*	@param logger				Optional logger used to issue loading logs.
			*
			*	@return true if a file could be loaded, else false.
			*/
			bool loadFromFile(fs::path const&	pathToSettingsFile,
							  ILogger*			logger = nullptr)	noexcept;
	};
}