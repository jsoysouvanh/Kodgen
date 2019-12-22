#pragma once

#include <vector>

#include "ParsingError.h"
#include "InfoStructures/ClassInfo.h"
#include "InfoStructures/EnumInfo.h"

namespace refureku
{
	struct ParsingResult
	{
		std::vector<ClassInfo>		classes;
		std::vector<EnumInfo>		enums;
		
		std::vector<ParsingError>	parsingErrors;
	};
}