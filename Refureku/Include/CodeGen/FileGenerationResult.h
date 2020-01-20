#pragma once

#include <vector>

#include "CodeGen/FileGenerationError.h"
#include "Parsing/ParsingError.h"

namespace refureku
{
	struct FileGenerationResult
	{
		/**
		*	This boolean is false if the generation directory path is empty, if the directory could not be created
		*	or if the path is valid but doesn't point to a directory (i.e. points to a file, a link...), in which case
		*	the file generation aborts immediately without performing any further operation.
		*	In any other case, the boolean will be true.
		*/
		bool								completed = false;

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