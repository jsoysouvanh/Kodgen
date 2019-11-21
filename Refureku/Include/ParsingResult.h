#pragma once

#include <vector>

#include "ParsingError.h"

namespace refureku
{
	struct ParsingResult
	{
		// List of classes
		// List of structs
		
		std::vector<ParsingError>	parsingErrors;
	};
}