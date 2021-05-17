#include "Kodgen/CodeGen/CodeGenerationModuleGroup.h"

#include <cassert>
#include <algorithm> //std::find

#include "Kodgen/CodeGen/CodeGenerationModule.h"
#include "Kodgen/CodeGen/CodeGenerationData.h"

using namespace kodgen;

void CodeGenerationModuleGroup::addModule(CodeGenerationModule& generationModule) noexcept
{
	//Add modules sorted by generation order
	_generationModules.insert
	(
		std::upper_bound(_generationModules.begin(), _generationModules.end(), &generationModule, [](CodeGenerationModule const* lhs, CodeGenerationModule const* rhs)
			{
				return lhs->generationOrder < rhs->generationOrder;
			}),
		&generationModule
	);
}

bool CodeGenerationModuleGroup::removeModule(CodeGenerationModule& generationModule) noexcept
{
	auto it = std::find(_generationModules.cbegin(), _generationModules.cend(), &generationModule);

	if (it != _generationModules.cend())
	{
		_generationModules.erase(it);

		return true;
	}

	return false;
}

bool CodeGenerationModuleGroup::generateCode(EntityInfo const& entity, CodeGenerationData& data, std::string& out_errorMessage) noexcept
{
	out_errorMessage.clear();

	//Generate code for each module
	for (CodeGenerationModule* generationModule : _generationModules)
	{
		if (!generationModule->generateCode(entity, data, out_errorMessage))
		{
			//Fill with an error message if the generation module didn't do so
			if (out_errorMessage.empty())
			{
				out_errorMessage = "Generation module failed to generate code but did not provide any error message.";
			}

			return false;
		}
	}

	return true;
}