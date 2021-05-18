#include "Kodgen/CodeGen/CodeGenModuleGroup.h"

#include <cassert>
#include <algorithm> //std::find

#include "Kodgen/CodeGen/CodeGenModule.h"
#include "Kodgen/CodeGen/CodeGenData.h"

using namespace kodgen;

void CodeGenModuleGroup::addModule(CodeGenModule& generationModule) noexcept
{
	//Add modules sorted by generation order
	_generationModules.insert
	(
		std::upper_bound(_generationModules.begin(), _generationModules.end(), &generationModule, [](CodeGenModule const* lhs, CodeGenModule const* rhs)
			{
				return lhs->generationOrder < rhs->generationOrder;
			}),
		&generationModule
	);
}

bool CodeGenModuleGroup::removeModule(CodeGenModule& generationModule) noexcept
{
	auto it = std::find(_generationModules.cbegin(), _generationModules.cend(), &generationModule);

	if (it != _generationModules.cend())
	{
		_generationModules.erase(it);

		return true;
	}
	
	return false;
}

bool CodeGenModuleGroup::generateCode(EntityInfo const* entity, CodeGenData& data, std::string& out_result) const noexcept
{
	out_result.clear();

	//Generate code for each module
	for (CodeGenModule* codeGenModule : _generationModules)
	{
		if (!codeGenModule->generateCode(entity, data, out_result))
		{
			return false;
		}
	}

	return true;
}