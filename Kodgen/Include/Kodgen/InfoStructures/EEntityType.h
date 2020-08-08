/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

namespace kodgen
{
	/**
	*	Enum representing type of an entity
	*	This avoids unnecessary dynamic casts and allows EntityInfo to be a non-polymorphic class
	*/
	enum class EEntityType : uint8
	{
		Undefined	= 0u,
		Class		= 1 << 0,
		Struct		= 1 << 1,
		Enum		= 1 << 2,
		Field		= 1 << 3,
		Method		= 1 << 4,
		EnumValue	= 1 << 5,
		Namespace	= 1 << 6
	};
}