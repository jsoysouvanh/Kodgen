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

bool CodeGenModuleGroup::generateCode(EntityInfo const& entity, CodeGenData& data, std::string const&	separator, std::string& out_result, std::string& out_errorMessage) noexcept
{
	out_errorMessage.clear();

	//Generate code for each module (except the last one to make sure the separator is not appended after the last module)
	for (int i = 0; i < _generationModules.size() - 1; i++)
	{
		if (!generateCodeInternal(_generationModules[i], entity, data, out_result, out_errorMessage))
		{
			return false;
		}

		//Append the separator between each code generation module
		out_result += separator;
	}

	//Generate code for the last module
	if (!_generationModules.empty() && !generateCodeInternal(_generationModules[_generationModules.size() - 1], entity, data, out_result, out_errorMessage))
	{
		return false;
	}

	return true;
}

bool CodeGenModuleGroup::generateCodeInternal(CodeGenModule* generationModule, EntityInfo const& entity, CodeGenData& data, std::string& out_result, std::string& out_errorMessage) noexcept
{
	if (!generationModule->generateCode(entity, data, out_result, out_errorMessage))
	{
		//Fill with an error message if the generation module didn't do so
		if (out_errorMessage.empty())
		{
			out_errorMessage = "Generation module failed to generate code but did not provide any error message.";
		}

		return false;
	}

	return true;
}