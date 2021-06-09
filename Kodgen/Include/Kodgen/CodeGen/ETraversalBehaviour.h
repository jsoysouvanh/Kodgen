/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

/**
*	This enum controls how entities are traversed.
*/
enum class ETraversalBehaviour
{
	/**
	*	Recursively traverse the entities contained in the current entity, using
	*	the same visitor and environment.
	*/
	Recurse = 0,

	/**
	*	Continues the entities traversal with the next sibling entity without visiting
	*	nested entities.
	*/
	Continue,

	/**
	*	Cancel the traversal on the siblings of the same entity type and resume
	*	it with the next sibling of a different type.
	*/
	Break,

	/**
	*	Abort the entity traversal but makes the generateCode method return true (success).
	*/
	AbortWithSuccess,

	/**
	*	Abort the entity traversal and make the generateCode method return false (failure).
	*/
	AbortWithFailure
};