/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#ifndef NDEBUG

#define KODGEN_DEBUG	1
#define KODGEN_RELEASE	0

#else	//RELEASE

#define KODGEN_DEBUG	0
#define KODGEN_RELEASE	1

#endif

//Set macro if RTTI is enabled
#if defined(__clang__)

#if __has_feature(cxx_rtti)
#define RTTI_ENABLED
#endif

#elif defined(__GNUG__)

#if defined(__GXX_RTTI)
#define RTTI_ENABLED
#endif

#elif defined(_MSC_VER)

#if defined(_CPPRTTI)
#define RTTI_ENABLED
#endif

#endif

#define KODGEN_VERSION_MAJOR 1
#define KODGEN_VERSION_MINOR 1
#define KODGEN_VERSION_PATCH 0