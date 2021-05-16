#include "Kodgen/CodeGen/Macro/MacroFileGenerationUnit.h"

using namespace kodgen;

void MacroFileGenerationUnit::generateCodeInternal(FileParsingResult const& parsingResult, FileGenerationResult& out_genResult) noexcept
{
	for (NamespaceInfo const& n : parsingResult.namespaces)
	{

	}

	//TODO: iterate over all entities & each module & call module.generateCode(data);
}

bool MacroFileGenerationUnit::isUpToDate(fs::path const& sourceFile) const noexcept
{
	return false;
}