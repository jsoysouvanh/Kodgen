#pragma once

#include <vector>

#include "CodeGen/FileGenerationError.h"
#include "Parsing/ParsingError.h"

namespace refureku
{
	struct FileGenerationResult
	{
		/**
		*	List of errors which occured during files parsing.
		*	If a file has parsing error, no code is generated.
		*/
		std::vector<ParsingError>			parsingErrors;

		/**
		*	List of errors which occured when attempting to generate code.
		*/
		std::vector<FileGenerationError>	fileGenerationErrors;
	};
}