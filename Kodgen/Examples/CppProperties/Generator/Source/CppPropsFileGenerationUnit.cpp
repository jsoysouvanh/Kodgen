#include "CppPropsFileGenerationUnit.h"

using namespace kodgen;

fs::path CppPropsFileGenerationUnit::makeGeneratedFilePath(fs::path const& sourceFilePath) const noexcept
{
	return (settings->getOutputDirectory() / sourceFilePath.filename()).replace_extension(settings->generatedFilesExtension);
}

void CppPropsFileGenerationUnit::generateCodeInternal(kodgen::FileParsingResult const& parsingResult, kodgen::FileGenerationResult& /* out_genResult */) noexcept
{
	kodgen::GeneratedFile generatedFile(makeGeneratedFilePath(parsingResult.parsedFile), parsingResult.parsedFile);

	generatedFile.writeLines("#pragma once\n",
							 "/**",
							 "*	Source file: " + generatedFile.getSourceFilePath().string(),
							 "*/\n",
							 "#include \"" + settings->entityMacrosFilename + "\"\n");

	//TODO: Modules
}

bool CppPropsFileGenerationUnit::isUpToDate(fs::path const& sourceFile)	const noexcept
{
	assert(fs::exists(sourceFile));
	assert(fs::is_regular_file(sourceFile));

	fs::path generatedFilePath = makeGeneratedFilePath(sourceFile);

	return fs::exists(generatedFilePath) && fs::is_regular_file(generatedFilePath) && isFileNewerThan(generatedFilePath, sourceFile);
}