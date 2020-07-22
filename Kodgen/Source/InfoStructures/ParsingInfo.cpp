#include "InfoStructures/ParsingInfo.h"

#include <cassert>

using namespace kodgen;

bool ParsingInfo::hasErrorOccured() const noexcept
{
	return !parsingResult.errors.empty();
}

bool ParsingInfo::flushCurrentStructOrClass() noexcept
{
	if (currentStructOrClass.has_value())
	{
		switch (currentStructOrClass->entityType)
		{
			case EntityInfo::EType::Class:
				parsingResult.classes.emplace_back(std::move(currentStructOrClass.value()));
				break;

			case EntityInfo::EType::Struct:
				parsingResult.structs.emplace_back(std::move(currentStructOrClass.value()));
				break;

			default:
				assert(false);	//Should never reach this line
		}

		currentStructOrClass.reset();

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

bool ParsingInfo::flushCurrentNamespace() noexcept
{
	//TODO
	
	return true;
}

void ParsingInfo::reset() noexcept
{
	propertyParser.setup(parsingSettings.propertyParsingSettings);

	accessSpecifier			= EAccessSpecifier::Private;

	currentStructOrClass	= std::nullopt;
	currentEnum				= std::nullopt;
	currentNamespaces.clear();

	parsingResult.namespaces.clear();
	parsingResult.classes.clear();
	parsingResult.enums.clear();
	parsingResult.errors.clear();
}