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
	struct CodeGenData
	{
		/** Whole parsing result. */
		FileParsingResult const*	parsingResult;

		/** Logger used to log data in the code generation process. Can be nullptr. */
		ILogger*					logger	= nullptr;
	};
}