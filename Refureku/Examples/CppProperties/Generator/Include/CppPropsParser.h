#pragma once

#include <Parsing/Parser.h>

class CppPropsParser : public refureku::Parser
{
	protected:
	virtual void preParse(fs::path const& parseFile)											noexcept override;
	virtual void postParse(fs::path const& parseFile, refureku::ParsingResult const& result)	noexcept override;

	public:
		CppPropsParser()						noexcept;
		CppPropsParser(CppPropsParser const&)	= default;
		CppPropsParser(CppPropsParser&&)		= default;
		~CppPropsParser()						= default;
};