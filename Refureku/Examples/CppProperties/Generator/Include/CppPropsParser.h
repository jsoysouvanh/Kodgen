#pragma once

#include <Parsing/Parser.h>

class CppPropsParser : public refureku::Parser
{
	public:
		CppPropsParser()						noexcept;
		CppPropsParser(CppPropsParser const&)	= default;
		CppPropsParser(CppPropsParser&&)		= default;
		~CppPropsParser()						= default;
};