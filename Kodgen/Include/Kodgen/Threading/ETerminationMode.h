/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Kodgen/Misc/FundamentalTypes.h"

namespace kodgen
{
	enum class ETerminationMode : uint8
	{
		/** Finish all tasks applied to the thread pool. */
		FinishAll		= 0,

		/** Finish running tasks and discard tasks which have not started yet. */
		FinishCurrent	= 1 << 0
	};
}