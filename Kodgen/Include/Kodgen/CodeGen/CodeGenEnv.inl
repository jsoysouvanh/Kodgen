/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

inline FileParsingResult const* CodeGenEnv::getParsingResult() const noexcept
{
	return _parsingResult;
}

inline ILogger* CodeGenEnv::getLogger() const noexcept
{
	return _logger;
}