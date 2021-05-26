#include "Kodgen/Parsing/ParsingResults/ParsingResultBase.h"

using namespace kodgen;

void ParsingResultBase::appendResultErrors(ParsingResultBase& result) noexcept
{
	if (!result.errors.empty())
	{
		errors.insert(errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));

		result.errors.clear();
	}
}