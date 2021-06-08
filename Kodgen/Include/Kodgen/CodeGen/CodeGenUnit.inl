/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

template <typename CodeGenEnvType>
bool CodeGenUnit::generateCode(FileParsingResult const& parsingResult, CodeGenEnvType& env) noexcept
{
	static_assert(std::is_base_of_v<CodeGenEnv, CodeGenEnvType>, "env type must be a derived class of kodgen::CodeGenEnv.");
	static_assert(std::is_copy_constructible_v<CodeGenEnvType>, "The provided CodeGenEnv must be copy-constructible");

	return	preGenerateCode(parsingResult, env) &&
			generateCodeInternal(parsingResult, env) &&
			postGenerateCode(parsingResult, env);
}