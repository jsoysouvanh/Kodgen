#include "InfoStructures/ParsingInfo.h"

using namespace refureku;

bool ParsingInfo::hasErrorOccured() const noexcept
{
	return !parsingResult.parsingErrors.empty();
}

bool ParsingInfo::flushCurrentClass() noexcept
{
	if (currentClass.has_value())
	{
		parsingResult.classes.emplace_back(std::move(currentClass.value()));
		currentClass.reset();

		return true;
	}

	return false;
}

bool ParsingInfo::flushCurrentEnum() noexcept
{
	if (currentEnum.has_value())
	{
		parsingResult.enums.emplace_back(std::move(currentEnum.value()));
		currentEnum.reset();

		return true;
	}

	return false;
}