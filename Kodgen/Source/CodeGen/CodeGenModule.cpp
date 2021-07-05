#include "Kodgen/CodeGen/CodeGenModule.h"

#include <algorithm>

#include "Kodgen/CodeGen/PropertyCodeGen.h"
#include "Kodgen/CodeGen/CodeGenEnv.h"
#include "Kodgen/CodeGen/CodeGenHelpers.h"

using namespace kodgen;

bool CodeGenModule::initialize(CodeGenEnv& env) noexcept
{
	for (PropertyCodeGen* propertyCodeGen : _propertyCodeGenerators)
	{
		if (!propertyCodeGen->initialize(env))
		{
			return false;
		}
	}

	return true;
}

void CodeGenModule::addPropertyCodeGen(PropertyCodeGen& propertyRule) noexcept
{
	_propertyCodeGenerators.push_back(&propertyRule);
}

bool CodeGenModule::removePropertyCodeGen(PropertyCodeGen const& propertyRule) noexcept
{
	auto it = std::find(_propertyCodeGenerators.cbegin(), _propertyCodeGenerators.cend(), &propertyRule);

	if (it != _propertyCodeGenerators.cend())
	{
		_propertyCodeGenerators.erase(it);

		return true;
	}

	return false;
}

ETraversalBehaviour CodeGenModule::generateCode(EntityInfo const* entity, CodeGenEnv& env, std::string& inout_result) noexcept
{
	if (entity != nullptr)
	{
		//Abort the traversal with a failure if something wrong happened during property code generation
		//Otherwise, return the least prioritized ETraversalBehaviour value to give the full control to this method overload.
		return runPropertyCodeGenerators(*entity, env, inout_result) ? CodeGenHelpers::leastPrioritizedTraversalBehaviour : ETraversalBehaviour::AbortWithFailure;
	}

	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

int32 CodeGenModule::getGenerationOrder() const noexcept
{
	return 0u;
}

bool CodeGenModule::runPropertyCodeGenerators(EntityInfo const& entity, CodeGenEnv& env, std::string& inout_result) const noexcept
{
	Property const* currentProperty;

	//Iterate over all entity's properties
	for (uint8 i = 0u; i < entity.propertyGroup.properties.size(); i++)
	{
		currentProperty = &entity.propertyGroup.properties[i];

		//Try to generate code with each registered property generator
		for (PropertyCodeGen* propertyCodeGen : _propertyCodeGenerators)
		{
			if (propertyCodeGen->shouldGenerateCode(entity, *currentProperty, i))
			{
				if (!propertyCodeGen->generateCode(entity, *currentProperty, i, env, inout_result))
				{
					return false;
				}
			}
		}
	}

	return true;
}

std::vector<PropertyCodeGen*> const& CodeGenModule::getPropertyRules() const noexcept
{
	return _propertyCodeGenerators;
}