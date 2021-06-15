/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/Parsing/ParsingResults/FileParsingResult.h"
#include "Kodgen/Misc/ILogger.h"

namespace kodgen
{
	//Forward declaration
	class CodeGenUnit;

	class CodeGenEnv
	{
		//CodeGenUnit is the only class allowed to set the private fields directly.
		//Other classes must access the fields through getters.
		friend CodeGenUnit;

		private:
			/** Whole parsing result. */
			FileParsingResult const*	_parsingResult	= nullptr;

			/** Logger used to log during the code generation process. Can be nullptr. */
			ILogger*					_logger			= nullptr;
		
		public:
			virtual ~CodeGenEnv() = default;

			/**
			*	@brief Getter for the _parsingResult field.
			* 
			*	@return _parsingResult.
			*/
			inline FileParsingResult const*	getParsingResult()	const noexcept;

			/**
			*	@brief Getter for the _logger field.
			* 
			*	@return _logger.
			*/
			inline ILogger*					getLogger()			const noexcept;
	};

	#include "Kodgen/CodeGen/CodeGenEnv.inl"
}