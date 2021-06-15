/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/CodeGen/ETraversalBehaviour.h"

namespace kodgen
{
	class CodeGenHelpers
	{
		public:
			/**
			*	Least prioritized ETraversalBehaviour value for MacroCodeGen pipeline, which means
			*	any other ETraversalBehaviour will be chosen over this one.
			*/
			static ETraversalBehaviour leastPrioritizedTraversalBehaviour;

			CodeGenHelpers()	= delete;
			~CodeGenHelpers()	= delete;

			/**
			*	@brief	In the MacroCodeGen pipeline, there are multiple spots where several ETraversalBehaviour are produced,
			*			and the code has to decide which one to pick. This method is here to serve this purpose of choosing the
			*			right ETraversalBehaviour.
			*			AbortWithFailure is chosen over AbortWithSuccess, Recurse, Continue, Break.
			*			AbortWithSuccess is chosen over Recurse, Continue, Break.
			*			Recurse is chosen over Continue, Break.
			*			Continue is chosen over Break.
			* 
			*	@param lhs First ETraversalBehaviour to combine.
			*	@param rhs Second ETraversalBehaviour to combine.
			* 
			*	@return The most prioritized ETraversalBehaviour between the 2 provided ETraversalBehaviour values.
			*/
			static ETraversalBehaviour combineTraversalBehaviours(ETraversalBehaviour lhs, ETraversalBehaviour rhs) noexcept;
	};
}