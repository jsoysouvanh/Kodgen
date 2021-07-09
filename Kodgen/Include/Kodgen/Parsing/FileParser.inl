/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

inline FileParsingResult* FileParser::getParsingResult() noexcept
{
	return reinterpret_cast<FileParsingResult*>(getContext().parsingResult);
}

inline ParsingSettings& FileParser::getSettings() noexcept
{
	//The _settings pointer should ALWAYS holds a reference (created at construction)
	assert(_settings.use_count() != 0);

	return *_settings;
}