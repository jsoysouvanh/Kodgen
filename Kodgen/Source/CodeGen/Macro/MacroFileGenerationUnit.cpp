#include "Kodgen/CodeGen/Macro/MacroFileGenerationUnit.h"

#include <iostream>

using namespace kodgen;

bool MacroFileGenerationUnit::generateCodeInternal(FileParsingResult const& parsingResult, FileGenerationResult& out_genResult) noexcept
{
	CodeGenerationData data{parsingResult, logger};

	return foreachEntity([](EntityInfo const& entity, CodeGenerationData& data)
			{
				std::cout << "entity: " << entity.getFullName() << std::endl;

				return EIterationResult::Recurse;
			}, data) != EIterationResult::AbortWithFailure;
}

bool MacroFileGenerationUnit::isUpToDate(fs::path const& sourceFile) const noexcept
{
	return false;
}