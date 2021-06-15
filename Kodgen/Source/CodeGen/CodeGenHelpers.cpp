#include "Kodgen/CodeGen/CodeGenHelpers.h"

#include <array>
#include <cassert>
#include <cstddef>	//std::size_t

using namespace kodgen;

ETraversalBehaviour CodeGenHelpers::leastPrioritizedTraversalBehaviour = ETraversalBehaviour::Break;

ETraversalBehaviour CodeGenHelpers::combineTraversalBehaviours(ETraversalBehaviour lhs, ETraversalBehaviour rhs) noexcept
{
	static std::array<ETraversalBehaviour, 5> prioritizedValues =	{
																		ETraversalBehaviour::AbortWithFailure,
																		ETraversalBehaviour::AbortWithSuccess,
																		ETraversalBehaviour::Recurse,
																		ETraversalBehaviour::Continue,
																		ETraversalBehaviour::Break
																	};

	for (std::size_t i = 0; i < prioritizedValues.size(); i++)
	{
		if (lhs == prioritizedValues[i] || rhs == prioritizedValues[i])
		{
			return prioritizedValues[i];
		}
	}

	//Should never reach this point since the provided lhs and rhs must be equal to one of the array values
	assert(false);

	return ETraversalBehaviour::AbortWithFailure;
}