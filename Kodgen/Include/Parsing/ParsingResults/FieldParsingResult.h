#pragma once

#include "Parsing/ParsingResults/ParsingResultBase.h"
#include "InfoStructures/FieldInfo.h"
#include "Misc/Optional.h"

namespace kodgen
{
	struct FieldParsingResult : public ParsingResultBase
	{
		public:
			opt::optional<FieldInfo>	parsedField	= opt::nullopt;
	};
}