/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Kodgen/Properties/PropertyRule.h"

namespace kodgen
{
	class DefaultPropertyRule : public PropertyRule
	{
		protected:
			/** Name the property should have to be considered valid. */
			std::string	propertyName;

			/** EEntityType bitmask defining valid entities (bit it set for valid entities). */
			EEntityType	validEntityTypes;

		public:
			DefaultPropertyRule()										= delete;
			DefaultPropertyRule(std::string	const&	propertyName,
								EEntityType			validEntityTypes)	noexcept;
			DefaultPropertyRule(DefaultPropertyRule const&)				= default;
			DefaultPropertyRule(DefaultPropertyRule&&)					= default;
			virtual ~DefaultPropertyRule()								= default;

			virtual bool	isValid(EntityInfo const&	entity,
									uint8				propertyIndex)		const	noexcept override;
	};
}