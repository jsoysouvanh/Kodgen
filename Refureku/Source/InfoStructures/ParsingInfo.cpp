#include "InfoStructures/ParsingInfo.h"

using namespace refureku;

bool ParsingInfo::hasErrorOccured() const noexcept
{
	return !parsingResult.parsingErrors.empty();
}
//
//void ParsingInfo::setParsingSettings(ParsingSettings const* parsingSettings) noexcept
//{
//	_parsingSettings = parsingSettings;
//	propertyParser.setup(&_parsingSettings->propertyParsingSettings);
//}