#include "Kodgen/CodeGen/Macro/MacroCodeGenData.h"

using namespace kodgen;

MacroCodeGenData::MacroCodeGenData(FileParsingResult const& parsingResult, ILogger* logger, CodeGenModuleGroup* codeGenModuleGroup) noexcept:
	CodeGenData{parsingResult, logger},
	codeGenModuleGroup{codeGenModuleGroup},
	codeGenLocation{ECodeGenLocation::Count}
{
}