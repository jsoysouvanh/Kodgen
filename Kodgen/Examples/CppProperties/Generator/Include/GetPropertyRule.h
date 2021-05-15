/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Kodgen/Properties/DefaultPropertyRule.h"

class GetPropertyRule : public kodgen::DefaultPropertyRule
{
	public:
		GetPropertyRule()						noexcept;
		GetPropertyRule(GetPropertyRule const&)	= default;
		GetPropertyRule(GetPropertyRule&&)		= default;
		virtual ~GetPropertyRule()				= default;

		virtual bool	isValid(kodgen::EntityInfo const&	entity,
								kodgen::uint8				propertyIndex)	const noexcept override;
};