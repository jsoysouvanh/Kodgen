#pragma once

#include <vector>

#include "ParsingError.h"
#include "InfoStructures/StructClassInfo.h"
#include "InfoStructures/EnumInfo.h"

namespace kodgen
{
	struct ParsingResult
	{
		std::vector<StructClassInfo>	classes;
		std::vector<StructClassInfo>	structs;
		std::vector<EnumInfo>			enums;
		
		std::vector<ParsingError>		parsingErrors;
	};
}