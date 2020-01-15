#include "InfoStructures/ParsingInfo.h"

using namespace refureku;

bool ParsingInfo::hasErrorOccured() const noexcept
{
	return !parsingResult.parsingErrors.empty();
}